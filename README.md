# ProofNest Implementation

## Key Points
- **Decentralized Ownership**: ProofNest likely allows creators to prove ownership of digital work using blockchain, ensuring tamper-proof records.
- **Hyperledger Fabric**: It seems to use Hyperledger Fabric for a private blockchain, suitable for controlled access and privacy.
- **Tech Stack**: The implementation appears to combine React.js for the frontend, Golang for the backend, and SHA-256 hashing for content verification.
- **Privacy Focus**: Only hashes are stored on-chain, suggesting a design that prioritizes user privacy by avoiding full content uploads.

## Overview
ProofNest is a platform designed to help creators like freelancers, students, and developers prove they created digital content—such as documents, designs, or code—before anyone else. By using blockchain technology, it creates a timestamped record that’s hard to dispute, without needing lawyers or courts. The implementation provided here uses Hyperledger Fabric, a permissioned blockchain, to store these records securely.

## How It Works
You can upload a file or paste text into a web interface built with React.js. The system generates a unique code (a hash) for your content and sends it to a blockchain network managed by Hyperledger Fabric. This network records the hash with your identity and the time, creating a proof of ownership. Later, anyone can check the blockchain to verify who created it and when, without seeing the original content.

## Setup and Usage
To use ProofNest, you’d need to set up a Hyperledger Fabric network, deploy the provided code, and run the web application. The backend, written in Golang, connects to the blockchain, while the frontend lets users interact easily. The system is designed to be private and secure, storing only hashes, not full files.

---

# Detailed Implementation Guide for ProofNest

## Introduction
ProofNest addresses a common problem faced by creators: proving ownership of digital work in collaborative or freelance settings. By leveraging Hyperledger Fabric, a permissioned blockchain framework, ProofNest creates immutable, timestamped records of content hashes tied to user identities. This guide provides a complete end-to-end implementation, including chaincode, backend, frontend, and setup instructions, as requested.

## System Architecture
ProofNest consists of three main components:
- **Frontend (React.js)**: A user-friendly interface for uploading files or text, computing SHA-256 hashes, and interacting with the backend.
- **Backend (Golang)**: An API server that handles requests, manages user identities, and communicates with the blockchain using the Fabric Gateway client API.
- **Blockchain (Hyperledger Fabric)**: A private network running Fabric v2.5, with chaincode to store and retrieve hash records.

The system uses SHA-256 hashing to ensure content privacy, storing only hashes on-chain, not the original files or text.

## Prerequisites
Before implementing ProofNest, ensure you have:
- **Docker and Docker Compose**: For running the Fabric network.
- **Go (1.16+)**: For chaincode and backend development.
- **Node.js (16+)**: For the React frontend.
- **Hyperledger Fabric Binaries**: Available via fabric-samples.
- **Fabric CA Client**: For user enrollment.

## Setting Up Hyperledger Fabric
ProofNest uses the Hyperledger Fabric test network for development. Follow these steps to set it up:

1. **Clone fabric-samples**:
   ```bash
   git clone https://github.com/hyperledger/fabric-samples.git
   cd fabric-samples/test-network
   ```

2. **Start the Network**:
   ```bash
   ./network.sh up createChannel -ca
   ```
   This command starts a network with two organizations, an ordering service, and a channel named `mychannel`, using Certificate Authorities (CAs) for identity management.

3. **Enroll Users**:
   Register and enroll two users, Alice and Bob, for Org1:
   ```bash
   export FABRIC_CA_CLIENT_HOME=$PWD/organizations/peerOrganizations/org1.example.com/
   fabric-ca-client register --id.name alice --id.secret alicepw --id.type client
   mkdir -p organizations/peerOrganizations/org1.example.com/users/alice@msp
   fabric-ca-client enroll -u http://alice:alicepw@localhost:7054 --mspdir organizations/peerOrganizations/org1.example.com/users/alice@msp
   fabric-ca-client register --id.name bob --id.secret bobpw --id.type client
   mkdir -p organizations/peerOrganizations/org1.example.com/users/bob@msp
   fabric-ca-client enroll -u http://bob:bobpw@localhost:7054 --mspdir organizations/peerOrganizations/org1.example.com/users/bob@msp
   ```

## Chaincode Development
The chaincode, written in Go, manages hash registration and verification. It includes two functions:
- `registerHash`: Stores a hash with the caller’s identity and timestamp, ensuring uniqueness.
- `getHashInfo`: Retrieves the owner and timestamp for a given hash.

### Chaincode Code
```go
package main

import (
	"encoding/json"
	"fmt"
	"time"

	"github.com/hyperledger/fabric-chaincode-go/shim"
	"github.com/hyperledger/fabric-protos-go/peer"
	"github.com/hyperledger/fabric-chaincode-go/pkg/cid"
)

type ProofNestChaincode struct {
}

type HashInfo struct {
	User      string `json:"user"`
	Timestamp string `json:"timestamp"`
}

func (t *ProofNestChaincode) Init(stub shim.ChaincodeStubInterface) peer.Response {
	return shim.Success(nil)
}

func (t *ProofNestChaincode) Invoke(stub shim.ChaincodeStubInterface) peer.Response {
	function, args := stub.GetFunctionAndParameters()
	if function == "registerHash" {
		return t.registerHash(stub, args)
	} else if function == "getHashInfo" {
		return t.getHashInfo(stub, args)
	}
	return shim.Error("Invalid function name")
}

func (t *ProofNestChaincode) registerHash(stub shim.ChaincodeStubInterface, args []string) peer.Response {
	if len(args) != 1 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}
	hash := args[0]

	val, err := stub.GetState(hash)
	if err != nil {
		return shim.Error("Failed to get state")
	}
	if val != nil {
		return shim.Error("Hash already registered")
	}

	callerID, err := cid.GetID(stub)
	if err != nil {
		return shim.Error("Failed to get caller ID")
	}

	txTimestamp, err := stub.GetTxTimestamp()
	if err != nil {
		return shim.Error("Failed to get transaction timestamp")
	}
	timestamp := time.Unix(txTimestamp.GetSeconds(), int64(txTimestamp.GetNanos())).Format(time.RFC3339)

	hashInfo := HashInfo{
		User:      callerID,
		Timestamp: timestamp,
	}

	hashInfoJSON, err := json.Marshal(hashInfo)
	if err != nil {
		return shim.Error("Failed to marshal JSON")
	}

	err = stub.PutState(hash, hashInfoJSON)
	if err != nil {
		return shim.Error("Failed to put state")
	}

	return shim.Success(nil)
}

func (t *ProofNestChaincode) getHashInfo(stub shim.ChaincodeStubInterface, args []string) peer.Response {
	if len(args) != 1 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}
	hash := args[0]

	val, err := stub.GetState(hash)
	if err != nil {
		return shim.Error("Failed to get state")
	}
	if val == nil {
		return shim.Error("Hash not found")
	}

	return shim.Success(val)
}

func main() {
	err := shim.Start(new(ProofNestChaincode))
	if err != nil {
		fmt.Printf("Error starting chaincode: %s", err)
	}
}
```

### Deploying Chaincode
1. Place the chaincode in `fabric-samples/chaincode/proofnest`.
2. Deploy it to the network:
   ```bash
   ./network.sh deployCC -ccn proofnest -ccp ../chaincode/proofnest -ccl go
   ```

## Backend Development
The backend is a Golang application using the Gin framework and the Fabric Gateway client API. It provides two endpoints:
- `/register`: Registers a hash for a user.
- `/verify`: Verifies ownership of a hash.

### Backend Code
```go
package main

import (
	"crypto/tls"
	"crypto/x509"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"path/filepath"

	"github.com/gin-gonic/gin"
	"github.com/hyperledger/fabric-gateway/pkg/client"
	"github.com/hyperledger/fabric-gateway/pkg/identity"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials"
)

var userMSPs = map[string]string{
	"alice":   "/home/user/fabric-samples/test-network/organizations/peerOrganizations/org1.example.com/users/alice@msp",
	"bob":     "/home/user/fabric-samples/test-network/organizations/peerOrganizations/org1.example.com/users/bob@msp",
	"default": "/home/user/fabric-samples/test-network/organizations/peerOrganizations/org1.example.com/users/User1@org1.example.com/msp",
}

func loadIdentity(mspPath string) (*identity.X509Identity, error) {
	certPath := filepath.Join(mspPath, "signcerts", "cert.pem")
	certPEM, err := ioutil.ReadFile(certPath)
	if err != nil {
		return nil, err
	}
	block, _ := pem.Decode(certPEM)
	if block == nil {
		return nil, fmt.Errorf("failed to decode certificate PEM")
	}
	cert, err := x509.ParseCertificate(block.Bytes)
	if err != nil {
		return nil, err
	}
	id, err := identity.NewX509Identity("Org1MSP", cert)
	if err != nil {
		return nil, err
	}
	return id, nil
}

func loadSign(mspPath string) (identity.Sign, error) {
	keystorePath := filepath.Join(mspPath, "keystore")
	files, err := ioutil.ReadDir(keystorePath)
	if err != nil {
		return nil, err
	}
	var keyFile string
	for _, file := range files {
		if !file.IsDir() {
			keyFile = filepath.Join(keystorePath, file.Name())
			break
		}
	}
	if keyFile == "" {
		return nil, fmt.Errorf("no private key found in %s", keystorePath)
	}
	keyPEM, err := ioutil.ReadFile(keyFile)
	if err != nil {
		return nil, err
	}
	privKey, err := identity.PrivateKeyFromPEM(keyPEM)
	if err != nil {
		return nil, err
	}
	sign, err := identity.NewPrivateKeySign(privKey)
	if err != nil {
		return nil, err
	}
	return sign, nil
}

func newGrpcConnection() (*grpc.ClientConn, error) {
	tlsCertPath := "/home/user/fabric-samples/test-network/organizations/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt"
	tlsCertPEM, err := ioutil.ReadFile(tlsCertPath)
	if err != nil {
		return nil, err
	}
	certPool := x509.NewCertPool()
	if !certPool.AppendCertsFromPEM(tlsCertPEM) {
		return nil, fmt.Errorf("failed to add server CA's certificate")
	}
	config := &tls.Config{
		RootCAs: certPool,
	}
	creds := credentials.NewTLS(config)
	conn, err := grpc.Dial("localhost:7051", grpc.WithTransportCredentials(creds))
	if err != nil {
		return nil, err
	}
	return conn, nil
}

func registerHandler(c *gin.Context) {
	var req struct {
		Username string `json:"username"`
		Hash     string `json:"hash"`
	}
	if err := c.BindJSON(&req); err != nil {
		c.JSON(400, gin.H{"error": "Invalid request"})
		return
	}
	mspPath, ok := userMSPs[req.Username]
	if !ok {
		c.JSON(400, gin.H{"error": "Unknown user"})
		return
	}
	id, err := loadIdentity(mspPath)
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to load identity"})
		return
	}
	sign, err := loadSign(mspPath)
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to load sign"})
		return
	}
	conn, err := newGrpcConnection()
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to connect to peer"})
		return
	}
	defer conn.Close()
	gateway, err := client.Connect(id, client.WithSign(sign), client.WithClientConnection(conn))
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to connect to gateway"})
		return
	}
	defer gateway.Close()
	network := gateway.GetNetwork("mychannel")
	contract := network.GetContract("proofnest")
	_, err = contract.SubmitTransaction("registerHash", req.Hash)
	if err != nil {
		c.JSON(500, gin.H{"error": fmt.Sprintf("Failed to submit transaction: %s", err)})
		return
	}
	c.JSON(200, gin.H{"status": "success"})
}

func verifyHandler(c *gin.Context) {
	var req struct {
		Hash string `json:"hash"`
	}
	if err := c.BindJSON(&req); err != nil {
		c.JSON(400, gin.H{"error": "Invalid request"})
		return
	}
	mspPath := userMSPs["default"]
	id, err := loadIdentity(mspPath)
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to load identity"})
		return
	}
	sign, err := loadSign(mspPath)
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to load sign"})
		return
	}
	conn, err := newGrpcConnection()
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to connect to peer"})
		return
	}
	defer conn.Close()
	gateway, err := client.Connect(id, client.WithSign(sign), client.WithClientConnection(conn))
	if err != nil {
		c.JSON(500, gin.H{"error": "Failed to connect to gateway"})
		return
	}
	defer gateway.Close()
	network := gateway.GetNetwork("mychannel")
	contract := network.GetContract("proofnest")
	result, err := contract.EvaluateTransaction("getHashInfo", req.Hash)
	if err != nil {
		c.JSON(500, gin.H{"error": fmt.Sprintf("Failed to evaluate transaction: %s", err)})
		return
	}
	var hashInfo struct {
		User      string `json:"user"`
		Timestamp string `json:"timestamp"`
	}
	if err := json.Unmarshal(result, &hashInfo); err != nil {
		c.JSON(500, gin.H{"error": "Failed to unmarshal result"})
		return
	}
	c.JSON(200, hashInfo)
}

func main() {
	r := gin.Default()
	r.POST("/register", registerHandler)
	r.POST("/verify", verifyHandler)
	r.Run(":8080")
}
```

### Setup Instructions
1. Install dependencies:
   ```bash
   go get github.com/gin-gonic/gin
   go get github.com/hyperledger/fabric-gateway/pkg/client
   go get github.com/hyperledger/fabric-gateway/pkg/identity
   ```

2. Update `userMSPs` and `tlsCertPath` in the code with the correct paths from your fabric-samples directory.

3. Run the backend:
   ```bash
   go run main.go
   ```

## Frontend Development
The frontend is a React.js single-page application that allows users to register and verify hashes. It uses the Web Crypto API for SHA-256 hashing and Axios for API requests.

### Frontend Code
```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ProofNest</title>
  <script src="https://cdn.jsdelivr.net/npm/react@18.2.0/umd/react.development.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/react-dom@18.2.0/umd/react-dom.development.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/axios@1.4.0/dist/axios.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/@babel/standalone@7.22.5/babel.min.js"></script>
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
- **Containerization**: Package the backend and chaincode in Docker containers.
- **Orchestration**: Deploy using Kubernetes for scalability.
- **CI/CD**: Set up pipelines with GitHub Actions or Jenkins for automated testing and deployment.

### Example Docker Configuration
| Component | Dockerfile Example |
|-----------|--------------------|
| Backend   | `FROM golang:1.16`<br>`WORKDIR /app`<br>`COPY . .`<br>`RUN go build -o main`<br>`CMD ["./main"]` |
| Chaincode | Managed by Fabric’s chaincode-as-a-service model. |

## Security Considerations
- **Private Key Management**: Storing private keys in files is insecure for production. Use Hardware Security Modules (HSMs) or secure vaults.
- **Authentication**: The current implementation assumes usernames without authentication. Add OAuth or JWT for secure user management.
- **Network Access**: Restrict access to the Fabric network to authorized nodes only.

## Limitations
- **Scalability**: The test network is for development. Production requires a multi-node setup.
- **User Enrollment**: Manual enrollment is used here. Automate with a registration service in production.
- **NFT Certificates**: The project mentions ProofNest Certificates as NFTs, but this implementation omits them for simplicity. Future versions could integrate NFT standards like ERC-721 on Fabric-EVM.

## Conclusion
This implementation provides a functional ProofNest platform using Hyperledger Fabric v2.5, Golang, and React.js. It allows users to register and verify ownership of digital content securely, with hashes stored on a private blockchain. For production, enhance security, scalability, and user management as outlined.

## Key Citations
- [Hyperledger Fabric v2.5 Documentation](https://hyperledger-fabric.readthedocs.io/en/release-2.5/)
- [Fabric Gateway Client API Repository](https://github.com/hyperledger/fabric-gateway)
- [Hyperledger Fabric Samples Repository](https://github.com/hyperledger/fabric-samples)