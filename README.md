## Micro-Contract Generator for Informal Gigs: End-to-End Implementation

This document provides a comprehensive implementation of the Micro-Contract Generator, a web application designed to empower informal gig workers by simplifying the creation of legally sound contracts, enabling electronic signatures, and offering dispute resolution through a chatbot. The solution uses **React** for the frontend, **Go** for the backend, **MongoDB** for contract storage, **DocuSign** for e-signatures, and **Dialogflow** for the chatbot. The implementation ensures contracts are written in plain language using basic text formatting, with the potential for advanced NLP integration.

### Overview

Informal gig workers, such as dog walkers, tutors, or freelancers, often face challenges due to the lack of quick, legally binding agreements, leading to disputes or unpaid work. The Micro-Contract Generator addresses this by providing a web-based platform where users can:
- Generate customizable contracts based on inputs like task, pay, and timeline.
- Sign contracts electronically using DocuSign.
- Resolve disputes via a Dialogflow-powered chatbot.
- Store contracts securely in MongoDB.

The application is unique in its use of AI-driven clarity (via simple text formatting, with room for NLP enhancements) and its focus on informal work. It empowers gig workers, builds trust, and reduces financial risks.

### Tech Stack

| Component       | Technology                     | Purpose                              |
|-----------------|-------------------------------|--------------------------------------|
| Frontend        | React, Tailwind CSS, Axios    | User interface for contract creation, signing, and chatbot interaction |
| Backend         | Go (Gin framework)            | RESTful API for business logic       |
| Database        | MongoDB                       | Contract and user data storage       |
| E-Signature     | DocuSign API (via esign)       | Electronic signature functionality   |
| Chatbot         | Dialogflow                    | Dispute resolution via NLP           |

### Backend Implementation (Go)

The backend is built using Go with the [Gin framework](https://github.com/gin-gonic/gin) for handling HTTP requests. It integrates [MongoDB](https://www.mongodb.com/) for data storage, [DocuSign](https://developers.docusign.com/) for e-signatures via the [esign library](https://github.com/jfcote87/esign), and [Dialogflow](https://cloud.google.com/dialogflow) for the chatbot.

#### Backend Code

```go
package main

import (
	"context"
	"encoding/base64"
	"log"
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
	"cloud.google.com/go/dialogflow/apiv2"
	dialogflowpb "cloud.google.com/go/dialogflow/apiv2/dialogflowpb"
	"google.golang.org/api/option"
	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"github.com/jfcote87/esign"
)

// MongoDB connection
var collection *mongo.Collection

// Contract model
type Contract struct {
	ID          string      `bson:"_id,omitempty" json:"id"`
	Title       string      `bson:"title" json:"title"`
	Description string      `bson:"description" json:"description"`
	Pay         float64     `bson:"pay" json:"pay"`
	StartDate   string      `bson:"startDate" json:"startDate"`
	EndDate     string      `bson:"endDate" json:"endDate"`
	Parties     []Party     `bson:"parties" json:"parties"`
	Status      string      `bson:"status" json:"status"`
	Signatures  []Signature `bson:"signatures" json:"signatures"`
}

type Party struct {
	Name  string `bson:"name" json:"name"`
	Email string `bson:"email" json:"email"`
}

type Signature struct {
	Party  string `bson:"party" json:"party"`
	Signed bool   `bson:"signed" json:"signed"`
}

// DocuSign client
var dsClient *esign.Client

// Dialogflow client
var dfClient *dialogflowpb.SessionsClient

func main() {
	// Connect to MongoDB
	clientOptions := options.Client().ApplyURI("mongodb://localhost:27017")
	client, err := mongo.Connect(context.TODO(), clientOptions)
	if err != nil {
		log.Fatal(err)
	}
	collection = client.Database("gigContracts").Collection("contracts")

	// Initialize DocuSign client
	integrationKey := os.Getenv("DS_INTEGRATION_KEY")
	userId := os.Getenv("DS_USER_ID")
	rsaKey := os.Getenv("DS_RSA_KEY")
	dsClient, err = esign.New(integrationKey, userId, rsaKey)
	if err != nil {
		log.Fatalf("Failed to initialize DocuSign client: %v", err)
	}

	// Initialize Dialogflow client
	ctx := context.Background()
	dfClient, err = dialogflowpb.NewSessionsClient(ctx, option.WithCredentialsFile("path/to/service-account.json"))
	if err != nil {
		log.Fatalf("Failed to create Dialogflow client: %v", err)
	}

	// Set up Gin router
	r := gin.Default()
	r.POST("/contracts", createContract)
	r.GET("/contracts", getContracts)
	r.GET("/contracts/:id", getContract)
	r.POST("/contracts/:id/sign", signContract)
	r.POST("/chatbot", chatbotHandler)

	r.Run(":8080")
}

// Create a new contract
func createContract(c *gin.Context) {
	var contract Contract
	if err := c.ShouldBindJSON(&contract); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	contract.ID = generateID()
	contract.Status = "draft"
	_, err := collection.InsertOne(context.TODO(), contract)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusCreated, contract)
}

// Get all contracts
func getContracts(c *gin.Context) {
	cursor, err := collection.Find(context.TODO(), bson.M{})
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	var contracts []Contract
	if err = cursor.All(context.TODO(), &contracts); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, contracts)
}

// Get a specific contract
func getContract(c *gin.Context) {
	id := c.Param("id")
	var contract Contract
	err := collection.FindOne(context.TODO(), bson.M{"_id": id}).Decode(&contract)
	if err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "Contract not found"})
		return
	}
	c.JSON(http.StatusOK, contract)
}

// Initiate signing with DocuSign
func signContract(c *gin.Context) {
	id := c.Param("id")
	var contract Contract
	err := collection.FindOne(context.TODO(), bson.M{"_id": id}).Decode(&contract)
	if err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "Contract not found"})
		return
	}

	// Generate PDF (placeholder)
	pdfContent := []byte("Contract PDF content") // Replace with actual PDF generation
	pdfBase64 := base64.StdEncoding.EncodeToString(pdfContent)

	// Create DocuSign envelope
	envelope := esign.EnvelopeDefinition{
		EmailSubject: "Sign your contract",
		Status:       "sent",
		Documents: []esign.Document{
			{
				Name:           "contract.pdf",
				DocumentID:     "1",
				DocumentBase64: pdfBase64,
			},
		},
		Recipients: esign.Recipients{
			Signers: []esign.Signer{
				{
					Name:        contract.Parties[0].Name,
					Email:       contract.Parties[0].Email,
					RecipientID: "1",
					Tabs: esign.Tabs{
						SignHereTabs: []esign.SignHere{
							{
								DocumentID: "1",
								PageNumber: "1",
								XPosition:  "100",
								YPosition:  "100",
							},
						},
					},
				},
			},
		},
	}

	// Send envelope
	envelopeSummary, err := dsClient.Envelopes.Send(contract.ID, envelope)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "Envelope sent", "envelopeId": envelopeSummary.EnvelopeID})
}

// Chatbot handler
func chatbotHandler(c *gin.Context) {
	var request struct {
		Message string `json:"message"`
		Session string `json:"session"`
	}
	if err := c.ShouldBindJSON(&request); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	ctx := context.Background()
	req := dialogflowpb.DetectIntentRequest{
		Session: request.Session,
		QueryInput: &dialogflowpb.QueryInput{
			Text: &dialogflowpb.TextInput{
				Text:         request.Message,
				LanguageCode: "en-US",
			},
		},
	}
	response, err := dfClient.DetectIntent(ctx, &req)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{"response": response.QueryResult.FulfillmentText})
}

// Helper function to generate ID (placeholder)
func generateID() string {
	return "contract-" + string(rune(time.Now().UnixNano())) // Simple ID generation
}
```

#### Backend Features
- **Contract Creation**: Accepts JSON input to create contracts, stored in MongoDB with a unique ID and "draft" status.
- **Contract Retrieval**: Provides endpoints to list all contracts or fetch a specific contract by ID.
- **E-Signature**: Integrates with DocuSign to send contracts for signing, using a placeholder PDF (requires actual PDF generation for production).
- **Chatbot**: Processes user messages via Dialogflow, detecting intents and returning responses for dispute resolution.
- **MongoDB Schema**: Stores contracts with fields for title, description, pay, dates, parties, status, and signatures.

#### Setup Requirements
- Install Go dependencies: `go get github.com/gin-gonic/gin go.mongodb.org/mongo-driver cloud.google.com/go/dialogflow/apiv2 github.com/jfcote87/esign`.
- Set environment variables for DocuSign (`DS_INTEGRATION_KEY`, `DS_USER_ID`, `DS_RSA_KEY`) and Dialogflow (service account JSON file).
- Run MongoDB locally or on a cloud instance (`mongodb://localhost:27017`).
- Start the backend: `go run main.go`.

### Frontend Implementation (React)

The frontend is a single-page React application styled with [Tailwind CSS](https://tailwindcss.com/) and uses [Axios](https://axios-http.com/) for API calls. It provides interfaces for contract creation, viewing, signing, and chatbot interaction.

#### Frontend Code

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Micro-Contract Generator</title>
  <script src="https://cdn.jsdelivr.net/npm/react@17/umd/react.development.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/react-dom@17/umd/react-dom.development.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/@babel/standalone/babel.min.js"></script>
  <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
</head>
<body>
  <div id="root"></div>
  <script type="text/babel">
    const { useState, useEffect } = React;

    const App = () => {
      const [contracts, setContracts] = useState([]);
      const [showChatbot, setShowChatbot] = useState(false);
      const [messages, setMessages] = useState([]);
      const [newMessage, setNewMessage] = useState('');

      useEffect(() => {
        fetchContracts();
      }, []);

      const fetchContracts = async () => {
        try {
          const response = await axios.get('http://localhost:8080/contracts');
          setContracts(response.data);
        } catch (error) {
          console.error(error);
        }
      };

      const handleCreateContract = async (contract) => {
        try {
          await axios.post('http://localhost:8080/contracts', contract);
          fetchContracts();
        } catch (error) {
          console.error(error);
        }
      };

      const handleSignContract = async (id) => {
        try {
          await axios.post(`http://localhost:8080/contracts/${id}/sign`);
          alert('Signing initiated');
        } catch (error) {
          console.error(error);
        }
      };

      const handleSendMessage = async () => {
        if (newMessage.trim() === '') return;
        setMessages([...messages, { text: newMessage, sender: 'user' }]);
        try {
          const response = await axios.post('http://localhost:8080/chatbot', {
            message: newMessage,
            session: 'projects/your-project-id/agent/sessions/some-session-id',
          });
          setMessages((prev) => [...prev, { text: response.data.response, sender: 'bot' }]);
        } catch (error) {
          console.error(error);
        }
        setNewMessage('');
      };

      return (
        <div className="container mx-auto p-4">
          <h1 className="text-2xl font-bold mb-4">Micro-Contract Generator</h1>
          <ContractForm onSubmit={handleCreateContract} />
          <h2 className="text-xl font-semibold mt-6 mb-2">Contracts</h2>
          <ul className="list-disc pl-5">
            {contracts.map((contract) => (
              <li key={contract.id} className="mb-2">
                {contract.title} - {contract.status}
                <button
                  onClick={() => handleSignContract(contract.id)}
                  className="ml-4 bg-blue-500 text-white px-2 py-1 rounded"
                >
                  Sign
                </button>
              </li>
            ))}
          </ul>
          <button
            onClick={() => setShowChatbot(!showChatbot)}
            className="mt-4 bg-green-500 text-white px-4 py-2 rounded"
          >
            {showChatbot ? 'Close Chatbot' : 'Open Chatbot'}
          </button>
          {showChatbot && (
            <Chatbot
              messages={messages}
              onSend={handleSendMessage}
              newMessage={newMessage}
              setNewMessage={setNewMessage}
            />
          )}
        </div>
      );
    };

    const ContractForm = ({ onSubmit }) => {
      const [title, setTitle] = useState('');
      const [description, setDescription] = useState('');
      const [pay, setPay] = useState(0);
      const [startDate, setStartDate] = useState('');
      const [endDate, setEndDate] = useState('');
      const [parties, setParties] = useState([{ name: '', email: '' }]);

      const handleSubmit = (e) => {
        e.preventDefault();
        const contract = {
          title,
          description,
          pay: parseFloat(pay),
          startDate,
          endDate,
          parties,
          signatures: parties.map((party) => ({ party: party.name, signed: false })),
        };
        onSubmit(contract);
        setTitle('');
        setDescription('');
        setPay(0);
        setStartDate('');
        setEndDate('');
        setParties([{ name: '', email: '' }]);
      };

      const addParty = () => {
        setParties([...parties, { name: '', email: '' }]);
      };

      return (
        <div className="bg-gray-100 p-4 rounded">
          <h2 className="text-xl font-semibold mb-2">Create Contract</h2>
          <div>
            <input
              type="text"
              value={title}
              onChange={(e) => setTitle(e.target.value)}
              placeholder="Title"
              className="w-full p-2 mb-2 border rounded"
            />
            <textarea
              value={description}
              onChange={(e) => setDescription(e.target.value)}
              placeholder="Description"
              className="w-full p-2 mb-2 border rounded"
            />
            <input
              type="number"
              value={pay}
              onChange={(e) => setPay(e.target.value)}
              placeholder="Pay"
              className="w-full p-2 mb-2 border rounded"
            />
            <input
              type="date"
              value={startDate}
              onChange={(e) => setStartDate(e.target.value)}
              className="w-full p-2 mb-2 border rounded"
            />
            <input
              type="date"
              value={endDate}
              onChange={(e) => setEndDate(e.target.value)}
              className="w-full p-2 mb-2 border rounded"
            />
            {parties.map((party, index) => (
              <div key={index} className="flex mb-2">
                <input
                  type="text"
                  value={party.name}
                  onChange={(e) => {
                    const newParties = [...parties];
                    newParties[index].name = e.target.value;
                    setParties(newParties);
                  }}
                  placeholder="Party Name"
                  className="w-1/2 p-2 mr-2 border rounded"
                />
                <input
                  type="email"
                  value={party.email}
                  onChange={(e) => {
                    const newParties = [...parties];
                    newParties[index].email = e.target.value;
                    setParties(newParties);
                  }}
                  placeholder="Party Email"
                  className="w-1/2 p-2 border rounded"
                />
              </div>
            ))}
            <button
              type="button"
              onClick={addParty}
              className="bg-gray-500 text-white px-2 py-1 rounded mb-2"
            >
              Add Party
            </button>
            <button
              type="submit"
              onClick={handleSubmit}
              className="bg-blue-500 text-white px-4 py-2 rounded"
            >
              Create Contract
            </button>
          </div>
        </div>
      );
    };

    const Chatbot = ({ messages, onSend, newMessage, setNewMessage }) => {
      return (
        <div className="mt-4 bg-gray-100 p-4 rounded">
          <h2 className="text-xl font-semibold mb-2">Dispute Resolution Chatbot</h2>
          <div className="h-64 overflow-y-auto border p-2 mb-2">
            {messages.map((msg, index) => (
              <div
                key={index}
                className={`mb-2 ${msg.sender === 'user' ? 'text-right' : 'text-left'}`}
              >
                <span
                  className={`inline-block p-2 rounded ${
                    msg.sender === 'user' ? 'bg-blue-200' : 'bg-gray-200'
                  }`}
                >
                  {msg.text}
                </span>
              </div>
            ))}
          </div>
          <div className="flex">
            <input
              type="text"
              value={newMessage}
              onChange={(e) => setNewMessage(e.target.value)}
              onKeyPress={(e) => e.key === 'Enter' && onSend()}
              placeholder="Type your message..."
              className="w-full p-2 border rounded"
            />
            <button
              onClick={onSend}
              className="ml-2 bg-blue-500 text-white px-4 py-2 rounded"
            >
              Send
            </button>
          </div>
        </div>
      );
    };

    ReactDOM.render(<App />, document.getElementById('root'));
  </script>
</body>
</html>
```

#### Frontend Features
- **Contract Creation Form**: Allows users to input gig details (title, description, pay, dates, parties) and submit to create a contract.
- **Contract List**: Displays all contracts with their titles and statuses, with a button to initiate signing.
- **E-Signature**: Triggers the signing process by calling the backend's `/contracts/:id/sign` endpoint.
- **Chatbot Interface**: Provides a chat window for dispute resolution, sending messages to the backend for Dialogflow processing.
- **Styling**: Uses Tailwind CSS for a responsive, modern design.

#### Setup Requirements
- Save the HTML file and open it in a browser (ensure the backend is running at `http://localhost:8080`).
- The app uses CDN-hosted React, Axios, and Tailwind CSS, so no additional installation is required.
- Update the Dialogflow session ID in the chatbot API call to match your Dialogflow project.

### Database (MongoDB)

MongoDB stores contracts in the `gigContracts` database, `contracts` collection. The schema is:

```json
{
  "_id": "contract-123",
  "title": "Dog Walking Gig",
  "description": "Walk client's dog twice daily",
  "pay": 100.0,
  "startDate": "2025-04-25",
  "endDate": "2025-04-30",
  "parties": [
    {"name": "Alice", "email": "alice@example.com"},
    {"name": "Bob", "email": "bob@example.com"}
  ],
  "status": "draft",
  "signatures": [
    {"party": "Alice", "signed": false},
    {"party": "Bob", "signed": false}
  ]
}
```

#### Setup
- Install MongoDB locally or use a cloud service like [MongoDB Atlas](https://www.mongodb.com/cloud/atlas).
- Ensure the MongoDB URI in the backend code matches your setup.

### Integrations

#### DocuSign
The backend uses the [esign library](https://github.com/jfcote87/esign) to interact with DocuSign's eSignature API. It creates an envelope with a placeholder PDF and sends it to the contract parties for signing.

**Setup**:
- Create a DocuSign developer account at [DocuSign Developer](https://developers.docusign.com/).
- Generate an integration key, user ID, and RSA private key.
- Set these as environment variables in the backend.

**Note**: The current implementation uses a placeholder PDF. For production, integrate a PDF generation library like [gopdf](https://github.com/signintech/gopdf) to create contract PDFs.

#### Dialogflow
The chatbot uses Dialogflow's [Go client library](https://pkg.go.dev/cloud.google.com/go/dialogflow/apiv2) to detect intents and provide responses for dispute resolution.

**Setup**:
- Create a Dialogflow agent at [Dialogflow Console](https://dialogflow.cloud.google.com/).
- Set up intents for dispute resolution (e.g., "payment issue", "task not completed").
- Download a service account JSON key and specify its path in the backend code.
- Update the session ID in the frontend to match your Dialogflow project.

### Plain Language (NLP)
To ensure contracts are clear and simple, the implementation relies on user-provided inputs formatted into a straightforward structure. For advanced NLP, you could integrate a library like [TextRazor](https://www.textrazor.com/) or [Google Cloud Natural Language](https://cloud.google.com/natural-language) to analyze and simplify text, but this is not included in the current code.

### Setup Instructions

1. **Backend**:
   - Install Go: [Download Go](https://go.dev/dl/).
   - Install dependencies: `go get github.com/gin-gonic/gin go.mongodb.org/mongo-driver cloud.google.com/go/dialogflow/apiv2 github.com/jfcote87/esign`.
   - Configure environment variables for DocuSign and Dialogflow.
   - Run MongoDB locally or via a cloud service.
   - Start the backend: `go run main.go`.

2. **Frontend**:
   - Save the provided HTML file as `index.html`.
   - Open it in a browser with the backend running.
   - Ensure CORS is enabled in the backend if running on different ports (add `r.Use(cors.Default())` with `github.com/gin-contrib/cors`).

3. **Database**:
   - Set up MongoDB with the `gigContracts` database and `contracts` collection.
   - Verify the MongoDB URI in the backend code.

4. **Integrations**:
   - Configure DocuSign and Dialogflow as described above.
   - Test the e-signature and chatbot functionalities.

### Enhancements for Production
- **User Authentication**: Add JWT-based authentication to secure user data.
- **PDF Generation**: Use a library like [gopdf](https://github.com/signintech/gopdf) to generate contract PDFs.
- **Advanced NLP**: Integrate an NLP service to enhance contract clarity.
- **Session Management**: Implement proper session handling for the Dialogflow chatbot.
- **Error Handling**: Add robust error logging and user-friendly error messages.
- **HTTPS**: Use HTTPS for secure communication in production.

### Impact
The Micro-Contract Generator empowers gig workers by providing a tool to create legally sound agreements quickly, reducing the risk of disputes and unpaid work. The integration of e-signatures builds trust, while the chatbot offers a convenient way to mediate issues. By focusing on plain language, the app ensures accessibility for users without legal expertise.

### Key Citations
- [Gin Web Framework for Go](https://github.com/gin-gonic/gin)
- [MongoDB Official Website
