# ProofNest Implementation on ICP Blockchain

## Introduction

ProofNest is a decentralized platform designed to help creators prove ownership of digital content, such as documents, code, designs, or ideas, by registering a hash of the content on the Internet Computer Protocol (ICP) blockchain. This implementation replaces the original Hyperledger Fabric-based system with ICP, leveraging its scalable and decentralized architecture. The system uses Rust for the canister (ICP’s smart contract), Go for the backend, and React.js for the frontend, aligning with the user’s proficiency in Go.

## System Architecture

ProofNest consists of three main components:

- **Canister (Rust)**: A smart contract on ICP that stores and retrieves hash information (hash, user identity, timestamp).
- **Backend (Go)**: An API server that handles user requests, computes SHA-256 hashes, and interacts with the canister using the ICP agent for Go.
- **Frontend (React.js)**: A user interface for uploading files or text, computing hashes, and verifying ownership.

The system uses SHA-256 hashing to ensure content privacy, storing only hashes on-chain, not the original files or text.

## Prerequisites

Before implementing ProofNest, ensure you have:

- **IC SDK**: For deploying canisters (Install IC SDK).
- **Rust (1.64+)**: For canister development (Rust Installation).
- **Go (1.16+)**: For backend development (Go Installation).
- **Node.js (16+)**: For the React frontend (Node.js Installation).
- **dfx**: The ICP command-line tool, included with the IC SDK.

## Setting Up the Development Environment

1. **Install the IC SDK**:

   ```bash
   sh -ci "$(curl -fsSL https://internetcomputer.org/install.sh)"
   ```

   Verify installation:

   ```bash
   dfx --version
   ```

2. **Set Up a Local ICP Network**:

   ```bash
   dfx start --clean
   ```

3. **Create a New dfx Project**:

   ```bash
   dfx new proofnest
   cd proofnest
   ```

## Canister Development (Rust)

The canister, written in Rust, manages hash registration and verification. It includes two functions:

- `register_hash`: Stores a hash with the caller’s identity and timestamp, ensuring uniqueness.
- `get_hash_info`: Retrieves the owner and timestamp for a given hash.

### Canister Code

```rust
use ic_cdk::export::{
    candid::{CandidType, Deserialize},
    Principal,
};
use ic_cdk_macros::*;
use std::cell::RefCell;
use std::collections::HashMap;

// Define the structure for hash information
#[derive(CandidType, Deserialize, Clone)]
struct HashInfo {
    user: Principal,
    timestamp: u64,
}

// Use thread-local storage for the hash map
thread_local! {
    static HASH_MAP: RefCell<HashMap<String, HashInfo>> = RefCell::new(HashMap::new());
}

// Function to register a hash
#[update]
fn register_hash(hash: String) -> () {
    let caller = ic_cdk::caller();
    let timestamp = ic_cdk::api::time();
    HASH_MAP.with(|map| {
        let mut map = map.borrow_mut();
        // Ensure the hash is unique
        if map.contains_key(&hash) {
            panic!("Hash already registered");
        }
        map.insert(hash.clone(), HashInfo { user: caller, timestamp });
    });
}

// Function to retrieve hash information
#[query]
fn get_hash_info(hash: String) -> Option<HashInfo> {
    HASH_MAP.with(|map| {
        let map = map.borrow();
        map.get(&hash).cloned()
    })
}

// Define the canister's Candid interface
#[export_candid]
fn candid() -> String {
    r#"
    (service : {
        register_hash : (hash: text) -> ();
        get_hash_info : (hash: text) -> opt (record { user: principal; timestamp: nat64 });
    })
    "#
    .to_string()
}
```

### Deploying the Canister

1. Place the Rust code in `src/proofnest_backend/src/lib.rs`.
2. Update `dfx.json` to include the canister:

   ```json
   {
     "canisters": {
       "proofnest_backend": {
         "main": "src/proofnest_backend/src/lib.rs",
         "type": "rust"
       }
     }
   }
   ```
3. Deploy the canister locally:

   ```bash
   dfx deploy
   ```
4. Note the canister ID for backend integration.

## Backend Development (Go)

The backend is a Go application using the Gin framework and the ICP agent for Go. It provides two endpoints:

- `/register`: Registers a hash for a user.
- `/verify`: Verifies ownership of a hash.

### Backend Code

```go
package main

import (
    "crypto/sha256"
    "encoding/hex"
    "io"
    "net/http"
    "time"

    "github.com/aviate-labs/agent-go"
    "github.com/aviate-labs/agent-go/candid"
    "github.com/aviate-labs/agent-go/principal"
    "github.com/gin-gonic/gin"
)

// Define the HashInfo structure matching your Rust canister
type HashInfo struct {
    User      principal.Principal `ic:"user"`
    Timestamp uint64              `ic:"timestamp"`
}

func main() {
    // Set up the ICP agent - uses default identity
    config := agent.Config{
        FetchRootKey: true, // Set to false in production
        Host:         "http://localhost:4943", // Local replica
        // For production: "https://ic0.app"
    }
    
    // For local development, we can use anonymous identity
    identity := agent.AnonymousIdentity{}
    
    // Create agent
    agent, err := agent.New(identity, config)
    if err != nil {
        panic(err)
    }

    // Replace with your actual canister ID
    canisterID, err := principal.Decode("uxrrr-q7777-77774-qaaaq-cai") // Your deployed canister ID
    if err != nil {
        panic(err)
    }

    // Set up Gin router
    r := gin.Default()

    // Enable CORS
    r.Use(func(c *gin.Context) {
        c.Writer.Header().Set("Access-Control-Allow-Origin", "*")
        c.Writer.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE")
        c.Writer.Header().Set("Access-Control-Allow-Headers", "Content-Type, Content-Length, Accept-Encoding, Authorization")
        if c.Request.Method == "OPTIONS" {
            c.AbortWithStatus(204)
            return
        }
        c.Next()
    })

    // Endpoint to register a hash
    r.POST("/register", func(c *gin.Context) {
        // Read file or text from request
        file, _, err := c.Request.FormFile("file")
        if err != nil {
            c.JSON(http.StatusBadRequest, gin.H{"error": "Failed to read file"})
            return
        }
        defer file.Close()

        // Compute SHA-256 hash
        hash := sha256.New()
        if _, err := io.Copy(hash, file); err != nil {
            c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to compute hash"})
            return
        }
        hashStr := hex.EncodeToString(hash.Sum(nil))

        // Call canister to register hash
        ctx, cancel := context.WithTimeout(context.Background(), 30*time.Second)
        defer cancel()
        
        _, err = agent.UpdateCall(ctx, canisterID, "register_hash", []interface{}{hashStr})
        if err != nil {
            c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to register hash: " + err.Error()})
            return
        }
        c.JSON(http.StatusOK, gin.H{"message": "Hash registered successfully", "hash": hashStr})
    })

    // Endpoint to verify a hash
    r.POST("/verify", func(c *gin.Context) {
        var req struct {
            Hash string `json:"hash"`
        }
        if err := c.BindJSON(&req); err != nil {
            c.JSON(http.StatusBadRequest, gin.H{"error": "Invalid request"})
            return
        }

        // Call canister to get hash info
        ctx, cancel := context.WithTimeout(context.Background(), 30*time.Second)
        defer cancel()
        
        response, err := agent.QueryCall(ctx, canisterID, "get_hash_info", []interface{}{req.Hash})
        if err != nil {
            c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to retrieve hash info: " + err.Error()})
            return
        }

        // Parse the response - in aviate-labs/agent-go we need to handle the optional value
        var result []HashInfo
        _, err = candid.Decode(response, &result)
        if err != nil {
            c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to decode response: " + err.Error()})
            return
        }
        
        // Check if we got a result (optional type handling)
        if len(result) == 0 {
            c.JSON(http.StatusNotFound, gin.H{"error": "Hash not found"})
            return
        }
        
        // Return the result
        c.JSON(http.StatusOK, gin.H{
            "user":      result[0].User.String(),
            "timestamp": result[0].Timestamp,
        })
    })

    // Run the server
    r.Run(":8080")
}
```

### Setup Instructions

1. Install dependencies:

   ```bash
   go get github.com/gin-gonic/gin
   go get github.com/dfinity/internetcomputer-go/agent
   go get github.com/dfinity/internetcomputer-go/candid
   ```
2. Update `canisterId` with the actual ID from the deployed canister.
3. Run the backend:

   ```bash
   go run main.go
   ```

## Frontend Development (React.js)

The frontend is a React.js single-page application that allows users to register and verify hashes. It uses the Web Crypto API for SHA-256 hashing and Axios for API requests.

### Frontend Code

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ProofNest</title>
  <script src="https://cdn.jsdelivr.net/npm/react@17/umd/react.development.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/react-dom@17/umd/react-dom.development.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/@babel/standalone/babel.min.js"></script>
  <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
</head>
<body>
  <div id="root"></div>
  <script type="text/babel">
    const { useState } = React;

    const App = () => {
      const [activeTab, setActiveTab] = useState('register');

      return (
        <div className="container mx-auto p-4">
          <h1 className="text-3xl font-bold mb-4">ProofNest</h1>
          <div className="tabs mb-4">
            <button
              className={`px-4 py-2 ${activeTab === 'register' ? 'bg-blue-500 text-white' : 'bg-gray-200'}`}
              onClick={() => setActiveTab('register')}
            >
              Register
            </button>
            <button
              className={`px-4 py-2 ${activeTab === 'verify' ? 'bg-blue-500 text-white' : 'bg-gray-200'}`}
              onClick={() => setActiveTab('verify')}
            >
              Verify
            </button>
          </div>
          {activeTab === 'register' ? <Register /> : <Verify />}
        </div>
      );
    };

    const Register = () => {
      const [username, setUsername] = useState('');
      const [text, setText] = useState('');
      const [file, setFile] = useState(null);
      const [message, setMessage] = useState('');

      const computeHash = async (data) => {
        const encoder = new TextEncoder();
        const dataBuffer = encoder.encode(data);
        const hashBuffer = await crypto.subtle.digest('SHA-256', dataBuffer);
        const hashArray = Array.from(new Uint8Array(hashBuffer));
        const hashHex = hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
        return hashHex;
      };

      const handleRegister = async () => {
        let data;
        if (file) {
          const reader = new FileReader();
          reader.onload = async (event) => {
            data = event.target.result;
            const hash = await computeHash(data);
            sendRegisterRequest(hash);
          };
          reader.readAsText(file);
        } else {
          data = text;
          const hash = await computeHash(data);
          sendRegisterRequest(hash);
        }
      };

      const sendRegisterRequest = async (hash) => {
        try {
          const response = await axios.post('http://localhost:8080/register', { username, hash });
          setMessage(response.data.status);
        } catch (error) {
          setMessage('Error: ' + (error.response?.data?.error || 'Unknown error'));
        }
      };

      return (
        <div className="bg-white p-6 rounded shadow">
          <h2 className="text-2xl mb-4">Register Hash</h2>
          <input
            type="text"
            placeholder="Username"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            className="border p-2 mb-4 w-full"
          />
          <textarea
            placeholder="Text"
            value={text}
            onChange={(e) => setText(e.target.value)}
            className="border p-2 mb-4 w-full"
          ></textarea>
          <input
            type="file"
            onChange={(e) => setFile(e.target.files[0])}
            className="mb-4"
          />
          <button
            onClick={handleRegister}
            className="bg-blue-500 text-white px-4 py-2 rounded"
          >
            Register
          </button>
          <p className="mt-4">{message}</p>
        </div>
      );
    };

    const Verify = () => {
      const [hash, setHash] = useState('');
      const [result, setResult] = useState(null);

      const handleVerify = async () => {
        try {
          const response = await axios.post('http://localhost:8080/verify', { hash });
          setResult(response.data);
        } catch (error) {
          setResult({ error: error.response?.data?.error || 'Unknown error' });
        }
      };

      return (
        <div className="bg-white p-6 rounded shadow">
          <h2 className="text-2xl mb-4">Verify Hash</h2>
          <input
            type="text"
            placeholder="Hash"
            value={hash}
            onChange={(e) => setHash(e.target.value)}
            className="border p-2 mb-4 w-full"
          />
          <button
            onClick={handleVerify}
            className="bg-blue-500 text-white px-4 py-2 rounded"
          >
            Verify
          </button>
          {result && (
            <div className="mt-4">
              {result.error ? (
                <p>Error: {result.error}</p>
              ) : (
                <div>
                  <p>User: {result.user}</p>
                  <p>Timestamp: {result.timestamp}</p>
                </div>
              )}
            </div>
          )}
        </div>
      );
    };

    ReactDOM.render(<App />, document.getElementById('root'));
  </script>
</body>
</html>
```

### Setup Instructions

1. Save the HTML file as `index.html`.
2. Serve it using a static file server (e.g., `npx serve`).
3. Ensure the backend is running at `http://localhost:8080`.

## Cloud-Native Enhancements

To make ProofNest cloud-native, consider:

- **Containerization**: Package the backend and frontend in Docker containers.
- **Orchestration**: Deploy using Kubernetes for scalability.
- **CI/CD**: Set up pipelines with GitHub Actions for automated testing and deployment.

### Example Docker Configuration

| Component | Dockerfile Example |
| --- | --- |
| Backend | `FROM golang:1.16`<br>`WORKDIR /app`<br>`COPY . .`<br>`RUN go build -o main`<br>`CMD ["./main"]` |
| Frontend | `FROM node:16`<br>`WORKDIR /app`<br>`COPY . .`<br>`RUN npm install`<br>`CMD ["npx", "serve"]` |

## Security Considerations

- **Identity Management**: Securely manage user identities using ICP’s Internet Identity or similar services.
- **Input Validation**: Validate all inputs to prevent injection attacks.
- **Canister Upgrades**: Plan for canister upgrades to maintain state (Canister Upgrades).

## Limitations

- **Go for Canisters**: Writing canisters in Go is experimental and not officially supported (Go Canister Efforts). Rust is used for stability.
- **Scalability**: ICP’s subnet architecture supports scaling, but test thoroughly for high loads.
- **Cost**: Canisters require cycles, which must be managed (ICP Cycles).

## Conclusion

This implementation provides a functional ProofNest platform on ICP, using Rust for the canister, Go for the backend, and React.js for the frontend. It allows users to register and verify ownership of digital content securely, with hashes stored on a decentralized blockchain. For production, enhance security, scalability, and cycle management as outlined.

## Key Citations

- Install the IC SDK for Canister Development
- Rust Installation Guide for Developers
- Go Installation Instructions
- Node.js Download and Installation
- Writing Smart Contracts on ICP in Go – A First Step
- Canister Upgrades and Storage Persistence
- ICP Overview and Cycles Management
