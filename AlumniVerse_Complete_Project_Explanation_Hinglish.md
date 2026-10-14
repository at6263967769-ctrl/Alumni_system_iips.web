# ALUMNIVERSE

### *Interactive Alumni Tree & Graph Intelligence Platform - Complete Technical Architecture, Codebase Deep Dive & Viva Preparation Guide*


| Project Property | Repository Evidence / Details |
| --- | --- |
| Project Name | AlumniVerse (IIPS DAVV Alumni System) |
| Target Audience | Students, Faculty, Viva Examiners, Project Review Committee |
| Language / Style | Hinglish / Conversational WhatsApp Style + Technical Deep Dive |
| Primary Tech Stack | C++17 (Crow REST API Engine), React 19, Vite, @xyflow/react, PostgreSQL Schema, Docker |
| Repository Path | d:\Alumni_system |
| Backend Executable | backend-cpp\build\alumniverse_backend.exe (Port 8080) |
| Frontend Dev Server | frontend (Vite on Port 3000 / 5173) |
| Unit Test Status | graph_tests.exe: 50 PASSED, 0 FAILED (Verified locally) |
| Document Purpose | Student ko zero se hero banana taaki wo sir ke har question ka confidently answer de sake |


> [!NOTE]
> **[VIVA GOLDEN RULE] Is document ka har ek word aur line aapke actual repository (d:\Alumni_system) ke code par based hai. Koi bhi file, endpoint, function, algorithm ya database table imagine/invent nahi kiya gaya hai. Jahan feature fully implemented hai wahan [IMPLEMENTED] likha hai, aur jahan koi gap hai wahan openly [PARTIAL] ya [NOT FOUND] likha hai taaki aap viva mein kabhi fasso nahi!**


---

# PART 1  -  Project Ka Overview & Real-World Problem


## 1.1 Project Kis Problem Ko Solve Karta Hai?

Har college ya university (jaise IIPS DAVV) mein hazaron students graduate hote hain aur alag-alag top companies (Google, Microsoft, Amazon, Razorpay, TCS wagairah) mein chale jaate hain. Lekin problem ye hai ki college ke paas unka record sirf boring Excel sheet ya PDF files mein pada rehta hai.

- Problem 1: Traditional alumni directories flat hoti hain: sirf naam, phone number aur email likha hota hai.
- Problem 2: Aapko pata nahi chalta ki kaunsa senior kis hierarchy mein kahan baitha hai (Department -> Degree -> Batch -> Alumni).
- Problem 3: Aapko ye nahi pata chalta ki kaunsa alumni kis doosre alumni ko jaanta hai (Professional Network Graph). Agar mujhe Google mein referral chahiye toh main kis senior ke through pahunch sakta hoon?
- Problem 4: Students aur Admins search nahi kar paate ki kaun 'Distributed Systems' ya 'Cloud' mein kaam kar raha hai.
AlumniVerse is problem ko solve karta hai ek interactive Tree aur Graph visualization ke through! Aap ek screen par dekh sakte ho ki IIPS ke kis department se kaun nikla, aur kaun kis company mein kya kaam kar raha hai. Saath hi, C++ backend ke high-performance Graph algorithms (BFS, Shortest Path) se aap 2 alumni ke beech ka connection bhi dhoondh sakte ho.


## 1.2 User aur Admin Website Par Kya Kar Sakte Hain?


| Role | Kya Action Perform Kar Sakte Hain (Frontend UI) | Backend Endpoint Called |
| --- | --- | --- |
| Guest / Viewer | Landing page dekh sakte hain, alumni directory browse kar sakte hain, tree & graph visually explore kar sakte hain. | GET /api/v1/tree, GET /api/v1/graph, GET /api/v1/alumni |
| Registered Alumni | Login/Register kar sakte hain, apna profile create ya edit kar sakte hain (skills, company, location update). | POST /api/v1/auth/login, PUT /api/v1/alumni/:id |
| Registered Alumni | Shortest Connection Path discovery chala sakte hain: 'Mere aur Google ke senior ke beech kaun mutual senior hai?' | GET /api/v1/graph/path?source=X&target=Y |
| System Administrator | Pending alumni profiles ko verify/approve ya reject kar sakte hain. | POST /api/v1/admin/alumni/:id/verify |
| System Administrator | Bulk CSV file upload karke sekdon alumni records ek click mein import kar sakte hain. | POST /api/v1/admin/import/csv |
| System Administrator | Do alumni ke beech verified connection edge create kar sakte hain (Colleague, Mentor, Classmate). | POST /api/v1/admin/connections |
| System Administrator | Platform ka security audit log dekh sakte hain ki kisne kya action perform kiya. | GET /api/v1/admin/audit-logs |
| System Administrator | Full analytics summary (Total verified alumni, active batches, top recruiters) inspect kar sakte hain. | GET /api/v1/admin/analytics |



## 1.3 Simple Real-World Example Se Samjhein

Maan lo ek fictional student hai Karan. Karan abhi MCA final year mein hai aur usko Google mein job ke liye apply karna hai. Traditional directory mein Karan sirf alumni ki email dhoondh sakta hai. Lekin AlumniVerse mein:

- Step 1: Tree Hierarchy: Karan Explorer page kholta hai aur 'Academic Tree' choose karta hai -> IIPS -> Department of Computer Applications -> MCA (2015-2020). Wahan usko 'Aarav Sharma' dikhta hai jo Google mein Staff Software Engineer hai.
- Step 2: Graph Mode: Karan 'Network Graph Mode' par click karta hai aur 'Find Shortest Path' button dabata hai.
- Step 3: Shortest Path: Source ID daalta hai 'Karan' aur Target ID daalta hai 'Aarav'. C++ backend turant unweighted BFS run karta hai aur path nikaalta hai: Karan -> Priya Patel (Microsoft) -> Aarav Sharma (Google).
- Step 4: Real-world Value: Karan ko pata chalta hai ki uski batchmate Priya Patel already Aarav ki purani colleague hai! Karan Priya ko message karke directly Aarav se warm referral le sakta hai. Ye hai visual graph intelligence ka power!

## 1.4 Actual Code Mein Features Ka Implementation Status

Examiner ya Sir ko project present karte time honesty sabse badi strength hoti hai. Yahan poore codebase ka exact status table hai jisse aapko clearly pata rahe ki kya ready hai aur kya future scope hai:


| Feature Name | Implementation Status | Evidence / File Path | Technical Explanation |
| --- | --- | --- | --- |
| C++ Crow REST Server | [IMPLEMENTED] | backend-cpp/src/main.cpp | Crow framework, multithreaded HTTP server on port 8080, CORS headers. |
| Academic Tree Hierarchy | [IMPLEMENTED] | backend-cpp/src/graph/AlumniTree.cpp | Institution -> Department -> Batch -> Alumni tree build aur BFS coordinate export. |
| Industry Tree Hierarchy | [IMPLEMENTED] | backend-cpp/src/graph/AlumniTree.cpp | Industry -> Company -> Alumni tree representation. |
| Tree Subtree Pruning | [IMPLEMENTED] | backend-cpp/src/graph/AlumniTree.cpp | filterTree() recursive function matches filter and prunes 0-match branches. |
| Graph Adjacency List | [IMPLEMENTED] | backend-cpp/src/graph/AlumniGraph.cpp | std::unordered_map<int, std::vector<GraphEdge>> representation. |
| BFS & DFS Traversal | [IMPLEMENTED] | backend-cpp/src/graph/AlumniGraph.cpp | Queue aur Stack based traversals with cycle protection via visited set. |
| Shortest Path Discovery | [IMPLEMENTED] | backend-cpp/src/graph/AlumniGraph.cpp | BFS with parent-map tracking. Reconstructs shortest unweighted path. |
| React Flow Visualizer | [IMPLEMENTED] | frontend/src/pages/ExplorerPage.jsx | @xyflow/react integration with custom nodes, zooming, panning, minimap. |
| Custom Canvas Nodes | [IMPLEMENTED] | frontend/src/components/CustomNodes.jsx | AlumniNode (avatar, badges, skills) aur HierarchyNode (count pills). |
| Live Multi-filter | [IMPLEMENTED] | frontend/src/components/FilterSidebar.jsx | Company, Batch, Verification status, Skills with OR/AND boolean logic. |
| Salted Password Hash | [IMPLEMENTED] | backend-cpp/include/utils/CryptoUtils.hpp | Pure C++ SHA-256 + 12-char random alphanumeric salt (salt$hash format). |
| Constant-time Verify | [IMPLEMENTED] | backend-cpp/include/utils/CryptoUtils.hpp | Bitwise OR verification to prevent timing attack side-channel leaks. |
| JWT Token Generator | [IMPLEMENTED] | backend-cpp/include/utils/CryptoUtils.hpp | HMAC-SHA256 signature, Base64Url encode/decode, 24-hr expiry validation. |
| CSV Bulk Import | [IMPLEMENTED] | backend-cpp/include/utils/CsvUtils.hpp | Custom parser with formula injection protection (sanitizeCell prepends '). |
| Admin Verification | [IMPLEMENTED] | backend-cpp/src/main.cpp:473 | Admins can mark pending profiles as verified or rejected. |
| Audit Logging System | [IMPLEMENTED] | backend-cpp/src/repositories/DataRepository.cpp | Logs actor, action, timestamp, entityType, and details. |
| Unit Test Suite (50 Tests) | [TESTED & PASSED] | backend-cpp/tests/GraphTests.cpp | 50 unit tests compiled and executed via graph_tests.exe. 100% pass. |
| PostgreSQL SQL Schema | [IMPLEMENTED] | database/migrations/001_initial_schema.sql | 11 relational tables, foreign keys, unique constraints, partial indexes. |
| PostgreSQL Driver Link | [HYBRID / IN-MEMORY] | backend-cpp/src/repositories/DataRepository.cpp | DataRepository has in-memory thread-safe engine with exact schema parity. DATABASE_URL is detected. |
| Real-time WebSockets / Chat | [NOT FOUND] | N/A (Future Scope) | Alumni ke beech direct live 1-to-1 chat abhi code mein nahi hai. |
| Email SMTP Verification | [NOT FOUND] | N/A (Future Scope) | Registration ke baad automated email verification link bhejna code mein nahi mila. |
| Payment Gateway / Donations | [NOT FOUND] | N/A (Future Scope) | Alumni donation ya payment portal code mein implemented nahi hai. |



---

# PART 2  -  Poore Project Ka Architecture & Request Lifecycle


## 2.1 3-Tier Layered Architecture Explanation

AlumniVerse ek standard industry-grade 3-Tier Client-Server Architecture par bana hua hai. Yahan har layer ka kaam bilkul separate (decoupled) hai:

Tier 1: Presentation Layer (Frontend): React 19 + Vite + TailwindCSS + @xyflow/react. Iska kaam hai user ko interactive UI dikhana, canvas par nodes render karna, user ke clicks aur filters capture karna, aur Axios ke through backend API ko call karna.

Tier 2: Application / Compute Layer (Backend): C++17 Crow REST API Engine (alumniverse_backend.exe). Ye project ka dimaag (Brain) hai! Ye incoming HTTP requests handle karta hai, JWT tokens verify karta hai, aur sabse important: in-memory Graph aur Tree data structures par high-speed traversal (BFS, DFS, Shortest Path, Tree Pruning) execute karta hai.

Tier 3: Persistence Layer (Data Storage): DataRepository Engine (C++) & PostgreSQL Relational Database (001_initial_schema.sql). Iska kaam hai saare alumni records, users, batches, career history, connections aur audit logs ko structured tables mein store karna.


## 2.2 Actual Architecture Flow Diagram (ASCII Representation)

Ye diagram project ke actual request aur response flow ko darshata hai:

`text
+---------------------------------------------------------------------------------------+
|                                TIER 1: FRONTEND (CLIENT)                               |
|   React 19 SPA | React Router DOM | TailwindCSS v4 | Lucide Icons | @xyflow/react       |
|                                                                                       |
|   [Explorer Page]       [Admin Dashboard]       [Directory Page]    [Profile Detail]  |
|          |                      |                      |                   |          |
|          +----------------------+----------------------+-------------------+          |
|                                 |                                                     |
|                  Axios API Client (src/api/client.js)                                 |
|            - Base URL: http://localhost:8080/api/v1 (or VITE_API_URL)                 |
|            - Attaches Bearer JWT token from localStorage                              |
+---------------------------------------------------------------------------------------+
                                          |
                      HTTP Request (JSON / GET / POST / PUT)
                      CORS Preflight Check (OPTIONS -> 204 No Content)
                                          v
+---------------------------------------------------------------------------------------+
|                           TIER 2: BACKEND ENGINE (C++17)                              |
|   Crow C++ Framework (crow_all.h) | Asio Standalone Networking | Port 8080            |
|                                                                                       |
|   src/main.cpp:                                                                       |
|     - setCorsHeaders(): Access-Control-Allow-Origin: *                                |
|     - parseFilterFromQuery(): Extracts search, department, batch, company, skills     |
|     - Route Handlers: /api/v1/tree, /api/v1/graph, /api/v1/alumni, /api/v1/auth...    |
|                                                                                       |
|   +-----------------------+ +-----------------------+ +-----------------------------+ |
|   |     AuthService       | |     TreeService       | |        GraphService         | |
|   | - validateToken(JWT)  | | - getTree(academic)   | | - getGraph(inducedSubgraph) | |
|   | - loginUser()         | | - getTree(industry)   | | - getNeighbors(alumniId)    | |
|   | - registerUser()      | | - getSubtree(nodeId)  | | - findShortestPath(src,dst) | |
|   +-----------------------+ +-----------------------+ +-----------------------------+ |
|               |                         |                            |                |
|   +-----------------------+ +-----------------------+ +-----------------------------+ |
|   |  CryptoUtils (SHA256) | |  AlumniTree (Tree DS) | |    AlumniGraph (Graph DS)   | |
|   | - Salted Password     | | - Root->Dept->Batch   | | - Adjacency List Map        | |
|   | - Constant-time cmp   | | - filterTree() prune  | | - BFS Shortest Path O(V+E)  | |
|   | - JWT HMAC-SHA256     | | - BFS coordinate calc | | - DFS Traversal O(V+E)      | |
|   +-----------------------+ +-----------------------+ +-----------------------------+ |
|                                         |                                             |
|                                         v                                             |
|   +---------------------------------------------------------------------------------+ |
|   |                 DataRepository (src/repositories/DataRepository.cpp)            | |
|   | - Thread-safe access via std::lock_guard<std::mutex>                            | |
|   | - Users, Institutions, Departments, Batches, Alumni, Companies, Audit Logs      | |
|   | - Auto-increment IDs (nextAlumniId_++), Foreign entity resolution               | |
|   +---------------------------------------------------------------------------------+ |
+---------------------------------------------------------------------------------------+
                                          |
                       Parity with Relational Database Schema
                                          v
+---------------------------------------------------------------------------------------+
|                             TIER 3: DATABASE STORAGE                                  |
|   PostgreSQL 16 Engine (Docker container: alumniverse_postgres on port 5432)          |
|   Schema Migration: database/migrations/001_initial_schema.sql (11 SQL Tables)        |
|   Seed Data: database/seeds/001_seed_data.sql                                         |
+---------------------------------------------------------------------------------------+
`


## 2.3 Frontend se Request Kaise Jaati Hai Aur Response Kaise Wapas Aata Hai?

WhatsApp Hinglish mein samajhte hain step-by-step ki jab user screen par koi button dabata hai toh andar kya hota hai:

- Step 1 (UI Trigger): Maan lo user ne Explorer page par 'Department of Computer Applications' par click kiya ya 'Search: Google' likha. React ka component (ExplorerPage.jsx) trigger hota hai aur loadVisualizationData() function ko call karta hai.
- Step 2 (API Client Dispatch): ExplorerPage.jsx call karta hai apiClient.getTree('academic', { company: 'Google' }). src/api/client.js ka Axios instance request banata hai: GET http://localhost:8080/api/v1/tree?hierarchy=academic&company=Google. Agar user logged in hai, toh localStorage se token nikaal kar header mein 'Authorization: Bearer <jwt>' chipka deta hai.
- Step 3 (CORS Preflight Check): Agar browser doosre origin (jaise port 3000) se request bhej raha hai, toh pehle ek OPTIONS request aati hai. C++ backend ka Crow router src/main.cpp:117 par OPTIONS request ko 204 No Content deta hai aur CORS headers (Access-Control-Allow-Origin: *) set karta hai, jisse browser request ko block nahi karta.
- Step 4 (Route Handling): Ab actual GET request C++ router ke CROW_ROUTE(app, '/api/v1/tree') handler par pahunchti hai (src/main.cpp:351). parseFilterFromQuery() query parameters se company='Google' ko extract karke FilterCriteria struct mein pack karta hai.
- Step 5 (C++ Tree Engine Execution): Handler call karta hai treeService->getTree('academic', filter). TreeService pehle check karta hai ki tree built hai ya nahi. Phir activeTree.filterTree(filter) call hota hai. AlumniTree.cpp recursive function filterSubtreeRecursive() chalaata hai: jin branches mein Google ka alumni nahi hai unhe cut (prune) kar deta hai, aur bache hue valid nodes ke liye level-by-level BFS chala kar canvas ke (X, Y) coordinates calculate karta hai!
- Step 6 (JSON Serialization): AlumniTree nlohmann::json format mein { nodes: [...], edges: [...] } return karta hai. src/main.cpp ka helper function successResponse() status 200 aur JSON stringify body bana kar return kar deta hai.
- Step 7 (UI Render): React ko JSON response milta hai: res.data.nodes aur res.data.edges. setNodes() aur setEdges() call hote hi @xyflow/react canvas par smooth animation ke saath alumni cards aur connecting lines render ho jaate hain!

---

# PART 3  -  Folder Aur Har Important File Ka Explanation

Is section mein aapke repository ke actual folder tree aur har single file ka complete WhatsApp-style explanation diya gaya hai.


## 3.1 Actual Repository Directory Tree

`text
d:\Alumni_system
├── .env.example                     # Environment variables template (PORT, DATABASE_URL, JWT_SECRET, VITE_API_URL)
├── .dockerignore                    # Files ignored during Docker image build
├── .gitignore                       # Git ignored files (build, node_modules, .env)
├── Dockerfile                       # Multi-stage root Docker build file
├── docker-compose.yml               # Orchestrates Postgres, C++ backend, and React frontend containers
├── render.yaml                      # Cloud deployment blueprint for Render.com hosting
├── run_backend.bat                  # Windows batch script to launch C++ backend executable
├── run_all.bat                      # Windows batch script to launch both C++ backend & React frontend
├── README.md                        # Project documentation, architecture overview, and setup guide
│
├── database                         # Database migrations and seed datasets
│   ├── migrations
│   │   └── 001_initial_schema.sql   # DDL script creating 11 relational tables, indexes, and constraints
│   └── seeds
│       ├── 001_seed_data.sql        # SQL seed data with institutions, batches, alumni, and connections
│       └── sample_alumni_import.csv # Sample CSV dataset for testing bulk admin import
│
├── backend-cpp                      # Core C++17 REST API and Data Structures Engine
│   ├── CMakeLists.txt               # CMake build recipe (defines alumniverse_backend and graph_tests targets)
│   ├── Dockerfile                   # Multi-stage Alpine/Debian C++ compilation container
│   ├── build                        # Pre-compiled binaries on Windows
│   │   ├── alumniverse_backend.exe  # Compiled C++ backend web server (Port 8080)
│   │   └── graph_tests.exe          # Compiled unit test runner (50 test assertions)
│   ├── include
│   │   ├── graph
│   │   │   ├── AlumniGraph.hpp      # Graph adjacency list declaration, BFS, DFS, Shortest Path prototypes
│   │   │   └── AlumniTree.hpp       # Tree hierarchy node declaration, academic/industry tree prototypes
│   │   ├── models
│   │   │   └── Models.hpp           # Data transfer structs (User, Alumni, Batch, Company, FilterCriteria)
│   │   ├── repositories
│   │   │   └── DataRepository.hpp   # In-memory thread-safe repository class declaration
│   │   ├── services
│   │   │   ├── AuthService.hpp      # Authentication & JWT validation service header
│   │   │   ├── GraphService.hpp     # Graph coordination service header
│   │   │   └── TreeService.hpp      # Tree coordination service header
│   │   ├── utils
│   │   │   ├── CryptoUtils.hpp      # Pure C++ SHA-256, HMAC-SHA256, salted password, JWT generator
│   │   │   └── CsvUtils.hpp         # CSV parser with formula injection sanitization
│   │   └── vendor
│   │       ├── crow_all.h           # Crow lightweight C++ web framework (header-only, 506 KB)
│   │       ├── asio.hpp / asio/     # Standalone Asio networking library
│   │       └── nlohmann/json.hpp    # Modern C++ JSON parser library (header-only, 920 KB)
│   ├── src
│   │   ├── main.cpp                 # Server entry point, route definitions, CORS, dependency injection
│   │   ├── graph
│   │   │   ├── AlumniGraph.cpp      # Graph implementation (BFS, DFS, shortest path, circular layout)
│   │   │   └── AlumniTree.cpp       # Tree implementation (academic/industry build, pruning, level BFS)
│   │   ├── repositories
│   │   │   └── DataRepository.cpp   # Seed data, thread-safe mutex CRUD operations, search filters
│   │   └── services
│   │       ├── AuthService.cpp      # User login, registration, and token validation logic
│   │       ├── GraphService.cpp     # Graph caching and graph API orchestration
│   │       └── TreeService.cpp      # Tree caching and tree API orchestration
│   └── tests
│       └── GraphTests.cpp           # 50 unit tests covering Graph, Tree, Crypto, and CSV sanitization
│
└── frontend                         # React 19 Client Application (Vite + TailwindCSS + @xyflow/react)
    ├── package.json                 # Node dependencies (React 19, @xyflow/react, axios, lucide-react)
    ├── vite.config.js               # Vite build configuration with @tailwindcss/vite plugin
    ├── Dockerfile                   # Multi-stage frontend Dockerfile (Vite build + Nginx alpine)
    ├── vercel.json                  # SPA routing configuration for Vercel deployment
    ├── index.html                   # HTML entry point with Google Fonts and title tag
    └── src
        ├── main.jsx                 # React root DOM mount point (ReactDOM.createRoot)
        ├── App.jsx                  # Main routing router with layout and route definitions
        ├── App.css / index.css      # Design tokens, custom scrollbars, dark mode glowing styles
        ├── api
        │   └── client.js            # Axios client with JWT interceptor and API methods
        ├── context
        │   └── AuthContext.jsx      # Authentication state context (user, token, role, login/logout)
        ├── pages
        │   ├── LandingPage.jsx      # High-conversion hero page with stats and CTA buttons
        │   ├── ExplorerPage.jsx     # Dual-mode Interactive Canvas (Tree & Graph React Flow)
        │   ├── DirectoryPage.jsx    # Card-based searchable directory with pagination and filters
        │   ├── DashboardPage.jsx    # User overview dashboard with metrics and quick shortcuts
        │   ├── ProfileDetailPage.jsx# Detailed alumni profile with career timeline and connections
        │   ├── AdminPage.jsx        # Admin portal: verification, CSV import, alumni add, audit logs
        │   └── LoginPage.jsx        # Login & registration authentication tabs
        └── components
            ├── Navbar.jsx           # Top navigation bar with active route highlight and user chip
            ├── Footer.jsx           # Footer with institution credits and useful links
            ├── CustomNodes.jsx      # React Flow custom nodes (AlumniNode & HierarchyNode)
            ├── FilterSidebar.jsx    # Slide-out multi-filter sidebar with OR/AND skills selector
            ├── AlumniDetailPanel.jsx# Slide-out alumni profile drawer when a node is clicked
            └── ShortestPathModal.jsx# Modal dialog to pick source and target alumni for BFS path finding
`


## 3.2 Root Configuration Files Ka WhatsApp Explanation


| File Path | Purpose & Kya Kaam Karti Hai | Caller & Callee Flow |
| --- | --- | --- |
| run_backend.bat | Ek single click Windows batch script. Ye check karti hai ki backend-cpp\build\alumniverse_backend.exe present hai ya nahi, aur usko port 8080 par launch karti hai. Isko double-click karte hi C++ server start ho jaata hai. | User isse double-click karta hai -> Calls alumniverse_backend.exe. |
| run_all.bat | Super-convenient script jo C++ backend aur React frontend dono ko alag-alag command prompt windows mein parallelly start kar deti hai. Ek window mein backend 8080 par chalta hai, doosri window mein 'npm run dev' 3000 par chalta hai. | User isse execute karta hai -> Starts alumniverse_backend.exe aur 'npm run dev'. |
| .env.example | Template file jo batati hai ki project ko chalane ke liye kaun-kaun se environment variables chahiye: PORT=8080, DATABASE_URL=postgresql://..., JWT_SECRET=..., VITE_API_URL=http://localhost:8080/api/v1. Real passwords ko git mein commit hone se rokti hai. | Developer isko copy karke .env banata hai -> Read by C++ (std::getenv) and Vite (import.meta.env). |
| docker-compose.yml | Pure multi-container setup ka master orchestrator. Isme 3 services define hain: 1) postgres:16-alpine (Port 5432), 2) backend (Dockerfile se C++ server build karta hai), 3) frontend (React build karke Nginx par host karta hai). Saath hi initial_schema.sql aur seeds.sql ko automatically execute karwata hai. | docker compose up command isse call karti hai -> Spawns 3 Docker containers. |
| render.yaml | Cloud deployment configuration (Infrastructure as Code) for Render.com. Isme backend web service aur frontend static site ki hosting commands and environment variables pre-configured hain. | Render Git webhook isse call karta hai cloud deploy karte time. |



## 3.3 Database Files Ka WhatsApp Explanation

Ye files database folder ke andar hain:

001_initial_schema.sql: database/migrations/001_initial_schema.sql: Ye file database ka naksha (Blueprint) hai. Isme 11 tables create hoti hain: users, institutions, departments, batches, alumni, companies, career_history, skills, alumni_skills, alumni_connections, aur audit_logs. Saath hi indexes aur foreign keys set hote hain.

001_seed_data.sql: database/seeds/001_seed_data.sql: Jab pehli baar database banta hai, toh testing ke liye initial demo data chahiye hota hai. Ye file IIPS DAVV institution, 3 departments (Computer Apps, Management, Commerce), 7 batches, 12 companies (Google, Microsoft, AWS, Razorpay...), aur 12 verified alumni records insert karti hai.

sample_alumni_import.csv: database/seeds/sample_alumni_import.csv: Admin page par CSV bulk import test karne ke liye sample CSV file hai. Isme columns hain: full_name, graduation_year, degree, department, company, job_title, industry, location, linkedin_url, skills.


---

# PART 4  -  C++ Backend Complete Deep Dive

Examiner ya Sir sabse zyada questions C++ backend par hi puchenge kyunki standard web projects mein log Node.js ya Python use karte hain, lekin AlumniVerse ne high performance aur custom Graph Data Structures ke liye C++17 use kiya hai. Yahan ek-ek cheez actual code references ke saath explained hai:


## 4.1 C++ Server Entry Point Aur Startup Flow

C++ server ka entry point backend-cpp/src/main.cpp ke main() function (line 96) se start hota hai. Server startup ka exact sequence ye hai:

- Step 1 (Layers Init): Line 101-105: Dependency Injection layers initialize hoti hain. Sabse pehle auto repo = std::make_shared<DataRepository>() banta hai. Phir AuthService, GraphService aur TreeService ko ye shared repo pointer pass kiya jaata hai. Isse sabhi services same central repository ko access kar paati hain.
- Step 2 (Crow App Instance): Line 107: crow::SimpleApp app create hoti hai. Crow ek header-only modern C++ microframework hai (jaise Python mein Flask ya Node.js mein Express hota hai).
- Step 3 (CORS Middleware): Line 110-122: Global CORS OPTIONS preflight handlers register hote hain jo kisi bhi browser se aane wali cross-origin request ko allow karte hain.
- Step 4 (Route Registration): Line 140-596: Saare REST endpoints (Auth, Alumni, Tree, Graph, Admin) lambda functions ke through register hote hain.
- Step 5 (Server Binding & Run): Line 601-611: std::getenv('PORT') se check kiya jaata hai ki koi custom port diya hai kya (default 8080). Phir app.bindaddr('0.0.0.0').port(port).multithreaded().run() call hota hai. Is command ke baad Crow server Asio event loop start karke multi-threaded requests sunna shuru kar deta hai!
`cpp
// backend-cpp/src/main.cpp:601-612
int port = 8080;
const char* envPort = std::getenv("PORT");
if (envPort && std::string(envPort).length() > 0) {
    try { port = std::stoi(envPort); } catch (...) {}
}

std::cout << "[INFO] Binding AlumniVerse HTTP Server to 0.0.0.0:" << port << "\n";
std::cout << "[INFO] REST API base: http://0.0.0.0:" << port << "/api/v1\n";
std::cout << "[INFO] Database status: " << repo->getDatabaseStatus() << "\n";

// Starts multithreaded Asio thread pool
app.bindaddr("0.0.0.0").port(port).multithreaded().run();
`


## 4.2 Multithreading Aur Thread Safety Ka Khel (std::mutex)

Crow server multithreaded() mode mein chalta hai. Iska matlab hai ki agar 10 users ek saath website kholte hain, toh 10 alag-alag CPU threads C++ functions ko call kar sakte hain. Agar do users ek hi time par naya alumni add karein toh memory corrupt (Race Condition) ho sakti hai!

AlumniVerse is problem ko solve karta hai std::mutex mtx_ aur std::lock_guard<std::mutex> se (backend-cpp/src/repositories/DataRepository.cpp:519). WhatsApp language mein samjhein: DataRepository ek room hai aur mutex uska lock hai. Jab bhi koi thread alumni create ya update karne aati hai, wo lock_guard banati hai. Room lock ho jaata hai, aur doosri thread ko bahar rukna padta hai. Jaise hi function khatam hota hai, lock_guard automatically unlock ho jaata hai (RAII principle)!


## 4.3 JSON Parsing Aur Status Codes

C++ mein standard library ke paas built-in JSON parser nahi hota. AlumniVerse use karta hai nlohmann/json (include/vendor/nlohmann/json.hpp). Incoming JSON ko read karne ke liye auto body = nlohmann::json::parse(req.body) use hota hai. Aur response bhejne ke liye jsonResponse() helper (main.cpp:28) use hota hai jo res.dump() karke string bhejta hai.

Status Codes: 200 OK: Request successfully complete hui (Data fetch, Tree/Graph query).

Status Codes: 201 Created: Naya resource create ho gaya (New Alumni created, New User registered).

Status Codes: 204 No Content: OPTIONS preflight check successful.

Status Codes: 400 Bad Request: Invalid JSON body ya missing required parameters (jaise source/target missing in /graph/path).

Status Codes: 401 Unauthorized: JWT token missing hai ya verifyJwt() fail ho gaya.

Status Codes: 403 Forbidden: Token toh valid hai lekin user ka role 'Admin' nahi hai (jaise normal alumni trying to delete someone).

Status Codes: 404 Not Found: Requested ID ka alumni ya company database mein nahi mila.


## 4.4 Complete REST API Endpoints Table

Repository mein implemented sabhi 18 endpoints ki complete master table:


| HTTP Method | Actual Endpoint Path | Purpose & Description | Input / Query Params | Output Format | Source File & Line |
| --- | --- | --- | --- | --- | --- |
| GET | / | Server status and API index | None | JSON {status, version, api_base} | src/main.cpp:140 |
| GET | /health & /api/v1/health | System health check & DB status | None | JSON {status, database: {connected, status}} | src/main.cpp:151-152 |
| POST | /api/v1/auth/register | User registration (Admin/Alumni/Viewer) | JSON {name, email, password, role} | JSON {token, user} | src/main.cpp:157 |
| POST | /api/v1/auth/login | User login with password hash check | JSON {email, password} | JSON {token, user} | src/main.cpp:181 |
| POST | /api/v1/auth/logout | Client session logout confirmation | None | JSON {message} | src/main.cpp:203 |
| GET | /api/v1/auth/me | Get current logged-in user details | Header: Authorization Bearer JWT | JSON User object | src/main.cpp:208 |
| GET | /api/v1/alumni | Paginated alumni list with multi-filters | search, dept, batch, company, skills, page, limit | JSON {items: [...], pagination: {...}} | src/main.cpp:221 |
| GET | /api/v1/alumni/:id | Get single alumni profile details | URL path id (integer) | JSON Alumni object with careerHistory & connections | src/main.cpp:244 |
| POST | /api/v1/alumni | Create new alumni profile (Admin/Alumni) | JSON Alumni payload + Bearer JWT | JSON created Alumni (201 Created) | src/main.cpp:253 |
| PUT | /api/v1/alumni/:id | Update existing alumni fields | JSON fields + Bearer JWT | JSON updated Alumni object | src/main.cpp:291 |
| DELETE | /api/v1/alumni/:id | Delete alumni and cascade relationships | Bearer JWT (Admin only) | JSON {message: deleted} | src/main.cpp:330 |
| GET | /api/v1/tree | Academic or Industry tree React Flow export | hierarchy=academic|industry, filters... | JSON {nodes: [...], edges: [...], stats} | src/main.cpp:351 |
| GET | /api/v1/tree/:id/subtree | Subtree isolation for specific node | URL nodeId, hierarchy, filters... | JSON {nodes: [...], edges: [...]} | src/main.cpp:359 |
| GET | /api/v1/tree/:id | Node details and immediate children | URL nodeId, hierarchy | JSON TreeNode object | src/main.cpp:367 |
| GET | /api/v1/graph | Network graph with induced filtered subgraph | filters... (skills, company, etc.) | JSON {nodes: [...], edges: [...], stats} | src/main.cpp:377 |
| GET | /api/v1/graph/:id/neighbors | Get 1-hop direct neighbors of an alumnus | URL alumniId | JSON {alumni, neighbors: [...]} | src/main.cpp:384 |
| GET | /api/v1/graph/:id/connections | Get confirmed relationship edges | URL alumniId | JSON {connections: [...], count} | src/main.cpp:390 |
| GET | /api/v1/graph/path | Shortest path discovery between 2 alumni | source=int, target=int | JSON {found, pathLength, pathNodeIds, visualization} | src/main.cpp:398 |
| GET | /api/v1/companies | List all registered companies | None | JSON Array of Company objects | src/main.cpp:416 |
| GET | /api/v1/companies/:id/alumni | Get alumni working at specific company | URL compId | JSON {company, alumni: [...], count} | src/main.cpp:424 |
| GET | /api/v1/metadata | System metadata (batches, depts, companies) | None | JSON {institutions, departments, batches, companies} | src/main.cpp:440 |
| GET | /api/v1/admin/analytics | Dashboard statistics (Admins only) | Bearer JWT | JSON {totalAlumni, verifiedAlumni, departments, ...} | src/main.cpp:467 |
| POST | /api/v1/admin/alumni/:id/verify | Verify pending alumni profile | Bearer JWT (Admin only), JSON {status} | JSON {message, status} | src/main.cpp:473 |
| POST | /api/v1/admin/import/csv | Bulk alumni import with sanitization | Bearer JWT (Admin), Body: Raw CSV text | JSON {imported_count, error_count, details} | src/main.cpp:498 |
| POST | /api/v1/admin/connections | Create connection edge between two alumni | Bearer JWT (Admin), JSON {src, dst, type} | JSON {message: confirmed} | src/main.cpp:556 |
| GET | /api/v1/admin/audit-logs | Security audit log history | Bearer JWT (Admin only) | JSON Array of AuditLog objects | src/main.cpp:584 |



## 4.5 Security Architecture: Passwords, JWT, CORS, Aur CSV Protection

AlumniVerse ka security system industry standards ko follow karta hai aur include/utils/CryptoUtils.hpp mein pure C++ mein implement kiya gaya hai:


## 1. Salted Password Hashing: Passwords ko plaintext mein store karna paap hai! AlumniVerse hashPassword() function use karta hai. Har user ke liye ek 12-character ka random salt (generateSalt(12)) generate hota hai. Password aur salt ko mila kar SHA-256 hash banta hai aur format hota hai: salt$hash. Agar do users ka password same 'Password123!' bhi ho, tab bhi unka hash alag-alag dikhega!


## 2. Constant-Time Password Verification: verifyPassword() function standard strcmp ya == use nahi karta! Kyunki == se hacker timing attack karke guess kar sakta hai ki kaunsa character match hua. AlumniVerse bitwise OR (diff |= actual[i] ^ expected[i]) use karta hai jo har baar exact constant time leta hai!


## 3. Pure C++ JWT Tokens: generateJwt() aur verifyJwt() pure C++ mein HMAC-SHA256 signature generate karte hain. Token format hota hai: base64(header).base64(payload).base64(signature). Server secret key se signature match karta hai aur expiry timestamp (exp) verify karta hai. Token 24 ghante mein automatically expire ho jaata hai.


## 4. Formula Injection Protection: Admin jab CSV import karta hai, toh Excel mein Formula Injection (CSV DDE attack) ho sakta hai agar koi cell '=cmd|...' se shuru ho. backend-cpp/include/utils/CsvUtils.hpp ka sanitizeCell() function check karta hai: agar cell '=', '+', '-', '@' se shuru ho, toh uske aage automatically single quote (') laga deta hai taaki Excel formula run na kare!

`text
// backend-cpp/include/utils/CryptoUtils.hpp:137-153
static bool verifyPassword(const std::string& password, const std::string& storedHash) {
    auto pos = storedHash.find('$');
    if (pos == std::string::npos) return false;

    std::string salt = storedHash.substr(0, pos);
    std::string expectedHash = storedHash.substr(pos + 1);
    std::string actualHash = sha256(password + salt);

    if (actualHash.length() != expectedHash.length()) return false;

    // Constant time comparison against timing attacks
    int diff = 0;
    for (size_t i = 0; i < actualHash.length(); ++i) {
        diff |= (actualHash[i] ^ expectedHash[i]);
    }
    return (diff == 0);
}
`


## 4.6 C++ Core Concepts Ki Master Table

Sir viva mein C++ concepts zaroor puchenge. Is table ko ratlo:


| C++ Concept | Actual Code Location | Kyun Use Hua? (Technical Reason) | Simple Analogy |
| --- | --- | --- | --- |
| std::shared_ptr<TreeNode> | include/graph/AlumniTree.hpp:45 | N-ary tree mein har parent apne children ko hold karta hai. shared_ptr automatic memory deallocation karta hai aur memory leak rokt hai. | Smart token: jab tak koi child ko use kar raha hai tab tak memory bachi rahegi. |
| std::unordered_map | include/graph/AlumniGraph.hpp:163 | Adjacency list ke liye O(1) average lookup time deta hai. Alumni ID se uske neighbors turant mil jaate hain. | Roll number se turant student ka record nikaalna. |
| std::unordered_set | src/graph/AlumniGraph.cpp:99 | BFS/DFS traversal mein cycle detect karne ke liye 'visited' nodes store karta hai in O(1) time. | Entry register: jo pehle se line mein lag gaya usse dubara add mat karo. |
| std::queue & std::stack | src/graph/AlumniGraph.cpp:101, 134 | BFS level-order traversal ke liye FIFO queue chahiye; DFS deep exploration ke liye LIFO stack chahiye. | Queue = Ticket counter line; Stack = Plate ke upar plate. |
| std::lock_guard<mutex> | src/repositories/DataRepository.cpp:519 | Multithreaded Crow server mein race conditions rokne ke liye automatic lock aur unlock (RAII) provide karta hai. | Washroom ka latch: andar ghuste hi lock, bahar aate hi unlock. |
| std::optional<T> | include/models/Models.hpp:191 | Nullable fields ke liye (jaise user_id ya end_date jo blank ho sakte hain). Null pointer crash se bachata hai. | Gift box jo khali bhi ho sakta hai ya jisme chocolate ho sakti hai. |
| const std::string& | src/graph/AlumniGraph.cpp:46 | Functions mein string pass karte time duplicate copy banane se bachata hai (Pass-by-reference), saving RAM & CPU cycles. | Photo copy banane ke bajaye original file dekhne ke liye dena. |
| Lambda with Capture | src/main.cpp:254 [repo, authService] | Crow route handlers inline define karne ke liye dependency injection pointers ko capture karte hain. | Worker ko toolkit thama dena taaki wo apna kaam kar sake. |



## 4.7 Backend Ki Current Limitations / Bugs / TODOs

Examiner ke saamne project ki limitations ko confidently acknowledge karne se aapki credibility 100x badh jaati hai:


## 1. In-Memory Persistence Parity: DataRepository in-memory store karta hai. Halanki PostgreSQL migration SQL ready hai aur DATABASE_URL detect hoti hai, lekin C++ executable ko direct libpqxx socket driver se link nahi kiya gaya taaki build self-contained rahe. Iska asar: Server restart karne par manually added alumni records reset ho kar default seed data par chale jaate hain.


## 2. Rate Limiting: Crow server mein per-IP rate limiting middleware (Token bucket algorithm) abhi code mein nahi hai. Production mein Nginx reverse proxy se rate limiting lagani padegi.


## 3. SSL/TLS Termination: Backend binary directly HTTP (cleartext) par serve karta hai. HTTPS SSL/TLS certificate handling Nginx ya Cloudflare level par honi chahiye.


---

# PART 5  -  Data Structures & Graph Algorithms Deep Dive


## 5.1 Tree Aur Graph Ka Difference (Real-World Example)

Tree aur Graph dono hierarchical aur network data structures hain, lekin inme zameen aasmaan ka farak hai:

- Tree: Tree ek hierarchical structure hai jisme STRICTLY 1 Root hota hai, koi Cycle (loop) nahi ho sakti, aur N nodes ke liye exactly N - 1 edges hote hain. Example: IIPS -> MCA Department -> Batch 2020 -> Aarav Sharma. Aarav ke do alag-alag batches nahi ho sakte, isliye ye Tree hai.
- Graph: Graph ek arbitrary network structure hai jisme koi bhi node kisi se bhi connect ho sakti hai! Isme cycles ho sakti hain (A jaanta hai B ko, B jaanta hai C ko, aur C jaanta hai A ko). Multiple parents aur shortcuts ho sakte hain. Example: Aarav jaanta hai Priya ko, Priya jaanti hai Rohan ko, aur Rohan jaanta hai Aarav ko.

## 5.2 Hierarchical Tree Implementation (AlumniTree)

AlumniTree.cpp do types ke hierarchies build karta hai:

Academic Tree: Institution -> Department -> Batch -> Alumni. buildAcademicTree() function bottom-up traversal karke har department aur batch ke andar total alumniCount calculate karta hai.

Industry Tree: Industry -> Company -> Alumni. buildIndustryTree() function companies ko unki industry ke hisab se group karta hai.

Tree Pruning: Subtree Pruning Logic (filterTree): Jab user filter karta hai (jaise company='Google'), toh filterSubtreeRecursive() recursive function har leaf alumni ko check karta hai. Agar kisi branch mein 0 matching alumni bache, toh wo poori branch ko cut (prune to nullptr) kar deta hai! Lekin valid alumni tak pahunchne wali parent hierarchy intact rehti hai.

Level-by-Level Coordinate Math: exportToReactFlowJson(): Tree nodes ko screen par sundar dikhane ke liye AlumniTree level-by-level BFS queue chalaata hai. Level 0 par Institution root, Level 1 par Departments, Level 2 par Batches, aur Level 3 par Alumni cards. posX aur posY math formulas se calculate hote hain taaki koi bhi card ek doosre ke upar overlap na ho!

`text
// backend-cpp/src/graph/AlumniTree.cpp:387-400
for (size_t lvl = 0; lvl < levels.size(); lvl++) {
    const auto& levelNodes = levels[lvl];
    double totalWidth = levelNodes.size() * NODE_SPACING_X;
    double startX = 600.0 - (totalWidth / 2.0);

    for (size_t i = 0; i < levelNodes.size(); i++) {
        const auto& node = levelNodes[i];
        double posX = startX + (i * NODE_SPACING_X);
        double posY = 80.0 + (lvl * LEVEL_HEIGHT);
        // Generates React Flow Node JSON with calculated (posX, posY)
    }
}
`


## 5.3 Graph Adjacency List Aur Traversal Algorithms

AlumniGraph.hpp mein graph ko Adjacency List se represent kiya gaya hai: std::unordered_map<int, std::vector<GraphEdge>> adjList_. Iska matlab: Har alumni ID ke aage ek list hai jisme uske saare direct neighbors ki targetId aur relationshipType likhi hui hai.

Undirected Graph: addEdge(1, 2, 'colleague') call hone par adjList_[1] mein 2 push hota hai aur adjList_[2] mein 1 push hota hai. Self loop (1 == 1) aur duplicate edges ko reject kar diya jaata hai.

BFS Traversal: std::queue<std::pair<int, int>> q aur std::unordered_set<int> visited use karta hai. Start node se shuru hoke level-by-level (breadth-wise) explore karta hai. Visited set hone ki wajah se agar graph mein cycle ho, tab bhi ye infinite loop mein nahi fasata!

DFS Traversal: std::stack<std::pair<int, int>> st aur visited set use karta hai. Ek branch mein gehraai (depth-wise) tak jaata hai aur jab aage koi unvisited node nahi bachti toh backtrack karta hai.


## 5.4 Shortest Path Discovery: BFS With Parent Tracking Dry Run

Shortest Path algorithm (AlumniGraph.cpp:162) Unweighted BFS use karta hai. Kyunki saare connections unweighted hain (each connection = 1 hop), isliye Dijkstra ki zaroorat nahi hai; BFS guaranteed shortest hop path return karta hai in O(V + E) time!

Maan lo hamare paas ek chhota demo graph hai:

Demo Graph: Node 1: Aarav Sharma (Google)

Demo Graph: Node 2: Priya Patel (Microsoft)

Demo Graph: Node 3: Rohan Verma (Razorpay)

Demo Graph: Node 5: Vikram Malhotra (Atlassian)

Demo Graph: Edges: 1 <-> 2, 2 <-> 3, 1 <-> 3 (Shortcut!), 3 <-> 5

Target: Find shortest path from Aarav (1) to Vikram (5).


| Step | Queue State (Front -> Back) | Current Node Dequeued | Visited Set | Parent Map [Child -> Parent] | Action Taken |
| --- | --- | --- | --- | --- | --- |
| 0 | [1] | - | {1} | {} | Start node 1 ko queue aur visited mein daala. |
| 1 | [] | 1 | {1} | {} | 1 ko dequeue kiya. 1 ke neighbors hain 2 aur 3. Dono unvisited hain. Queue mein push kiya aur parent[2]=1, parent[3]=1 set kiya. |
| 2 | [2, 3] | 1 | {1, 2, 3} | {2: 1, 3: 1} | Queue mein ab 2 aur 3 hain. |
| 3 | [3] | 2 | {1, 2, 3} | {2: 1, 3: 1} | 2 ko dequeue kiya. 2 ke neighbors: 1 (visited), 3 (already visited). Kuch naya add nahi hua. |
| 4 | [] | 3 | {1, 2, 3} | {2: 1, 3: 1} | 3 ko dequeue kiya. 3 ke neighbors: 1 (visited), 2 (visited), 5 (UNVISITED!). |
| 5 | [5] | 3 | {1, 2, 3, 5} | {2: 1, 3: 1, 5: 3} | 5 ko visited aur queue mein daala. parent[5]=3 set kiya. |
| 6 | [] | 5 | {1, 2, 3, 5} | {2: 1, 3: 1, 5: 3} | 5 ko dequeue kiya. Target 5 mil gaya! Loop terminate ho gaya found = true ke saath. |


Path Reconstruction Backwards: Target 5 se start kiya -> parent[5] = 3 -> parent[3] = 1. List bani: [5, 3, 1]. Is list ko std::reverse() kiya toh final shortest path mila: [1 -> 3 -> 5]! Notice karo ki path [1 -> 2 -> 3 -> 5] lamba tha (3 hops), lekin BFS ne direct shortcut chord [1 -> 3 -> 5] (2 hops) choose kiya!


## 5.5 Time and Space Complexity Master Table


| Algorithm / Operation | Time Complexity | Space Complexity | Where It Runs | Kyun Optimal Hai? |
| --- | --- | --- | --- | --- |
| Graph Add Vertex | O(1) average | O(1) | C++ AlumniGraph::addVertex | unordered_map hash insertion. |
| Graph Add Edge | O(deg(u) + deg(v)) | O(1) | C++ AlumniGraph::addEdge | Vector scan to reject duplicate edges, then push_back. |
| BFS Traversal | O(V + E) | O(V) | C++ AlumniGraph::bfs | Har vertex aur edge exactly ek baar visit hoti hai. |
| DFS Traversal | O(V + E) | O(V) | C++ AlumniGraph::dfs | Stack based exploration with visited set. |
| Shortest Path Discovery | O(V + E) | O(V) | C++ AlumniGraph::findShortestPath | Unweighted graph mein BFS se fast koi algorithm nahi ho sakti. |
| Tree Subtree Pruning | O(N) where N = nodes | O(H) where H = height | C++ AlumniTree::filterTree | Post-order bottom-up recursion drops unmatched subtrees in single pass. |
| React Flow Tree Layout | O(N) | O(N) | C++ AlumniTree::exportToReactFlowJson | Level-by-level BFS assigns X,Y positions in single pass. |



---

# PART 6  -  New Member/Alumni Add Karne Ka Complete End-to-End Flow

Viva mein sir ka favourite question: 'Agar main admin page par jaakar naya member add karun, toh front-to-back poora code trace karke dikhao'. Yahan fictional demo member 'Karan Sharma' ke saath exact 12-step trace diya gaya hai:


## 6.1 End-to-End Trace: Adding 'Karan Sharma'

- Step 1: Admin Form (Frontend): Admin user website par /admin URL par jaata hai aur 'Add Alumni' tab kholta hai (AdminPage.jsx:36). Form mein details fill karta hai:
- Full Name: Karan Sharma
- Grad Year: 2022
- Company: Infosys Technologies
- Job Title: Systems Engineer
- Industry: IT Consulting & Services
- Location: Indore, India
- Skills: Java, Spring Boot, SQL
- Step 2: Submit Handler (AdminPage.jsx:125): Admin 'Create Alumni Profile' button dabata hai. handleCreateAlumni(e) trigger hota hai. Form data ko format kiya jaata hai: graduation_year ko Number mein convert karte hain aur skills comma-separated string ko array mein split karte hain.
- Step 3: HTTP Dispatch (src/api/client.js:54): handleCreateAlumni() call karta hai apiClient.createAlumni(payload). Axios POST request bhejta hai:
- URL: http://localhost:8080/api/v1/alumni
- Header: Authorization: Bearer <jwt_token_of_admin>
- Body: JSON payload
- Step 4: C++ Route Matching (src/main.cpp:253): Crow HTTP server par POST /api/v1/alumni match hota hai. Lambda handler trigger hota hai: [repo, authService, graphService, treeService](const crow::request& req).
- Step 5: Authorization & Role Check (src/main.cpp:255): Handler request header se 'Authorization' nikaalta hai aur authService->validateToken(authHeader) call karta hai. Token verifyJwt() se check hota hai. Agar token missing ho ya role 'Viewer' ho, toh turant 403 Forbidden return ho jaata hai.
- Step 6: JSON Parsing & Struct Hydration (src/main.cpp:262): Alumni struct ka instance create hota hai: a.fullName = body.value('full_name', ''); a.currentCompany = body.value('current_company', ''); a.skills = body['skills']... Kyunki create karne wala Admin hai, isliye verificationStatus automatically 'verified' set ho jaata hai.
- Step 7: DataRepository Insertion (src/repositories/DataRepository.cpp:518): repo->createAlumni(a) call hota hai. std::lock_guard<std::mutex> lock(mtx_) lagta hai. nextAlumniId_ (e.g. 13) Karan ko assign hota hai. Institutions aur Batches map se departmentName aur degree resolve karke Karan ke record mein attach hoti hain. alumni_[13] aur alumniSkills_[13] map mein store ho jaate hain.
- Step 8: Audit Logging (src/main.cpp:282): repo->logAction(user->id, user->name, 'CREATE_ALUMNI', 'alumni', 13, 'Created alumni profile: Karan Sharma') call hota hai. Security audit log mein ek naya record add ho jaata hai.
- Step 9: Graph Synchronization (src/main.cpp:284): graphService->refreshGraph() call hota hai! GraphService DataRepository se saare alumni reload karta hai aur AlumniGraph mein graph_.addVertex(node) call karke Karan ko naye Graph Node ke roop mein insert kar deta hai!
- Step 10: Tree Synchronization (Auto On-Demand): TreeService mein ensureTreesBuilt() implement hai. Agli baar jab koi user Tree query karega, TreeService automatically naye alumni list se academicTree_.buildAcademicTree() chala dega jisse Karan batch-2022 ke andar attach ho jayega!
- Step 11: HTTP Response (src/main.cpp:285): Server 201 Created status ke saath created.toJson(false) JSON response frontend ko bhejta hai.
- Step 12: UI Update & Notification (AdminPage.jsx:138): Frontend par res.status === 'success' detect hota hai. Screen par green toast notification aati hai: 'Alumni profile for Karan Sharma created successfully!'. Form clear ho jaata hai aur Explorer page par Karan turant visible ho jaata hai!

## 6.2 Error Handling Aur Edge Cases

- Edge Case 1: Agar request mein JWT token na ho: C++ backend return karega 401 Unauthorized ('Unauthorized: invalid or missing authentication token').
- Edge Case 2: Agar user ka role 'Viewer' ho: Return karega 403 Forbidden ('Forbidden: Only authorized alumni or admins can create profiles').
- Edge Case 3: Agar JSON invalid ho (jaise trailing comma ya invalid types): catch (...) block trigger hoga aur return karega 400 Bad Request ('Invalid JSON payload for alumni creation').
- Edge Case 4: Agar graduation year 1950 se 2030 ke beech na ho: CSV import mein row reject ho jaati hai ('Invalid graduation year').

---

# PART 7  -  Database Explained (PostgreSQL Schema & In-Memory Sync)


## 7.1 Database Type Aur Connection Method

AlumniVerse project mein Database ka architecture do levels par configured hai:


## 1. Relational Database: Production / Docker Level: PostgreSQL 16 Alpine container (docker-compose.yml:4-23). Container start hote hi database/migrations/001_initial_schema.sql aur database/seeds/001_seed_data.sql automatically run ho kar 11 relational tables aur initial records create karte hain.


## 2. C++ Data Repository: C++ Runtime Level: DataRepository (backend-cpp/src/repositories/DataRepository.cpp). C++ backend mein ek thread-safe in-memory data engine hai jo exact PostgreSQL schema parity ke saath design kiya gaya hai. Constructor mein std::getenv('DATABASE_URL') check hota hai. Is architecture ka faayda ye hai ki local development mein bina PostgreSQL install kiye bhi backend standalone run ho sakta hai!


## 7.2 Relational Tables Master Breakdown (11 Tables)


| Table Name | Primary Key | Foreign Keys & References | Key Columns & Purpose |
| --- | --- | --- | --- |
| users | id (SERIAL) | None | name, email (UNIQUE), password_hash, role (admin/alumni/viewer). User authentication. |
| institutions | id (SERIAL) | None | name (UNIQUE), website. Top-level college root (e.g. IIPS DAVV). |
| departments | id (SERIAL) | institution_id -> institutions(id) | name. Unique constraint on (institution_id, name). |
| batches | id (SERIAL) | department_id -> departments(id) | degree, start_year, graduation_year. Unique on (dept, degree, grad_year). |
| alumni | id (SERIAL) | institution_id, department_id, batch_id, user_id | full_name, graduation_year, current_location, bio, linkedin_url, verification_status (pending/verified/rejected). |
| companies | id (SERIAL) | None | name (UNIQUE), website, industry. List of employers. |
| career_history | id (SERIAL) | alumni_id -> alumni(id), company_id -> companies(id) | job_title, start_date, end_date, is_current. Alumni employment history. |
| skills | id (SERIAL) | None | name (UNIQUE). Master skills dictionary (C++, System Design, AWS). |
| alumni_skills | (alumni_id, skill_id) | alumni_id -> alumni, skill_id -> skills | Many-to-many junction table associating alumni with their technical skills. |
| alumni_connections | id (SERIAL) | source_alumni_id -> alumni, target_alumni_id -> alumni | relationship_type (colleague/mentor/classmate/professional_connection), created_by -> users. |
| audit_logs | id (SERIAL) | actor_user_id -> users(id) | action (CREATE_ALUMNI, VERIFY_ALUMNI, etc.), entity_type, entity_id, details, created_at. |



## 7.3 Entity-Relationship (ER) Diagram (Textual Representation)

`text
+------------------+             +--------------------+             +------------------+
|   INSTITUTIONS   | 1         N |    DEPARTMENTS     | 1         N |     BATCHES      |
|  id (PK)         |------------>|  id (PK)           |------------>|  id (PK)         |
|  name            |             |  institution_id(FK)|             |  department_id(FK|
+------------------+             +--------------------+             |  degree, grad_yr |
                                                                    +------------------+
                                                                             | 1
                                                                             |
                                                                             v N
+------------------+             +--------------------+             +------------------+
|      USERS       | 1         1 |       ALUMNI       | N         M |      SKILLS      |
|  id (PK)         |<------------|  id (PK)           |<===========>|  id (PK)         |
|  email, password | (Optional)  |  batch_id (FK)     | alumni_skills  name (UNIQUE)   |
|  role            |             |  full_name, status |             +------------------+
+------------------+             +--------------------+
         |                                 | 1
         |                                 |
         |                                 v N
         | 1                     +--------------------+             +------------------+
         |                       |   CAREER_HISTORY   | N         1 |    COMPANIES     |
         |                       |  id (PK)           |------------>|  id (PK)         |
         v N                     |  alumni_id (FK)    |             |  name (UNIQUE)   |
+------------------+             |  company_id (FK)   |             |  industry        |
|    AUDIT_LOGS    |             |  is_current (BOOL) |             +------------------+
|  id (PK)         |             +--------------------+
|  actor_user_id   |                       |
|  action, details |             +---------+----------+
+------------------+             | ALUMNI_CONNECTIONS |
                                 |  source_alumni_id  |  (Self-referencing Many-to-Many
                                 |  target_alumni_id  |   Graph edges: colleague, mentor)
                                 +--------------------+
`


## 7.4 Special SQL Constraints & Indexes

Database schema mein real-world data integrity ke liye special constraints use kiye gaye hain:


## 1. Single Current Job Constraint: Partial Unique Index: Ek alumni ka ek time par sirf ek hi current job ho sakti hai. SQL: CREATE UNIQUE INDEX uq_alumni_single_current_job ON career_history(alumni_id) WHERE is_current = TRUE. Ye index ensure karta hai ki do active jobs simultaneously na ho sakein.


## 2. No Self-Connection Constraint: Check Constraint: Koi bhi alumni khud ko apna connection nahi bana sakta. SQL: CONSTRAINT chk_no_self_connection CHECK (source_alumni_id <> target_alumni_id).


## 3. Unique Connection Pair: Pair Unique Constraint: Do same alumni ke beech same type ka duplicate connection nahi ban sakta. SQL: CONSTRAINT uq_alumni_connection_pair UNIQUE (source_alumni_id, target_alumni_id, relationship_type).


---

# PART 8  -  Frontend & React Flow Integration


## 8.1 Frontend Architecture & Routing

Frontend modern React 19 single-page application (SPA) hai jo Vite build tool par run karta hai. Styling ke liye TailwindCSS v4 (@tailwindcss/vite) use hui hai aur routing ke liye React Router DOM v7.


| Route Path | Page Component | Purpose & User Experience |
| --- | --- | --- |
| / | LandingPage.jsx | Hero section with animated counters, platform highlights, and CTA button. |
| /dashboard | DashboardPage.jsx | Platform statistics, quick overview of top companies, and shortcuts. |
| /explorer | ExplorerPage.jsx | Main interactive canvas: Dual-mode Tree & Graph view with React Flow. |
| /directory | DirectoryPage.jsx | Card grid directory with live search, company filters, and pagination. |
| /alumni/:id | ProfileDetailPage.jsx | Complete alumni profile with career timeline, skills tags, and mutual connections. |
| /admin | AdminPage.jsx | Protected admin dashboard: verification tab, CSV import, alumni creation, audit logs. |
| /login | LoginPage.jsx | Clean tabs for user login and registration. |



## 8.2 React Flow Data Shape (Nodes & Edges)

C++ backend frontend ko React Flow ke standard JSON format mein data bhejta hai taaki @xyflow/react canvas par directly render ho sake:

`text
// C++ se React Flow ko aane wala Node structure
{
  "id": "alumni-1",
  "type": "alumniNode",
  "position": { "x": 600, "y": 320 },
  "data": {
    "id": 1,
    "name": "Aarav Sharma",
    "company": "Google",
    "job_title": "Staff Software Engineer",
    "industry": "Technology & Internet",
    "department": "Department of Computer Applications",
    "degree": "MCA",
    "graduation_year": 2020,
    "avatar_url": "https://images.unsplash.com/...",
    "verification_status": "verified",
    "location": "Bengaluru, India",
    "skills": ["C++", "Distributed Systems", "System Design"]
  }
}

// C++ se React Flow ko aane wala Edge structure
{
  "id": "e-1-2",
  "source": "1",
  "target": "2",
  "label": "colleague",
  "animated": false,
  "style": { "stroke": "#00D2FF", "strokeWidth": 2 }
}
`


## 8.3 Custom Canvas Components (CustomNodes.jsx)

Frontend ne default React Flow boxes use nahi kiye! frontend/src/components/CustomNodes.jsx mein 2 premium custom nodes hain:


## 1. AlumniNode: AlumniNode: Ek rich card jisme alumni ka avatar photo, verified blue badge, full name, company name (e.g. Google), job title, aur technical skills ke color pills dikhte hain. Card par hover karne par glowing shadow effect aata hai.


## 2. HierarchyNode: HierarchyNode: Tree view ke internal branches (Institution, Department, Batch) ke liye rounded rectangular badge. Iske andar child alumni count dikhta hai (e.g. 'MCA (2015-2020)  -  4 Alumni'). User is node par click karke poori branch ko isolate (zoom in) kar sakta hai!


## 8.4 Interactive Features: Filter Sidebar, Detail Drawer & Shortest Path Modal

Multi-criteria Filtering: FilterSidebar.jsx: Live search keyword, batch filter, company dropdown, aur skills pills. Sabse unique feature: Skills matching logic ko user 'OR' (kisi bhi ek skill match ho) ya 'AND' (saari selected skills match honi chahiye) mein toggle kar sakta hai!

Slide-Out Profile Drawer: AlumniDetailPanel.jsx: Jab user canvas par kisi Alumni node par click karta hai, right side se ek smooth drawer slide hota hai jisme alumni ki complete career history timeline, LinkedIn link, aur direct connections dikhte hain.

Shortest Connection Modal: ShortestPathModal.jsx: User Explorer page par 'Find Shortest Path' button click karta hai. Modal khulta hai jisme Source Alumni aur Target Alumni dropdowns se select hote hain. Backend /api/v1/graph/path ko call karta hai aur graph par path ko highlight karke render kar deta hai!


---

# PART 9  -  Run / Build / Deploy Guide & Actual Test Results


## 9.1 Prerequisites & Tool Versions

- Prerequisites: Operating System: Windows 10/11 (ya Linux / macOS)
- Prerequisites: C++ Compiler: GCC / MinGW (C++17 standard) ya MSVC
- Prerequisites: CMake: Version 3.15 or newer
- Prerequisites: Node.js: Version 18.x or 20.x with npm
- Prerequisites: Docker & Docker Compose: (Optional, for containerized multi-tier run)

## 9.2 Local Machine Par Run Karne Ke Exact Commands

`text
# --- OPTION A: ONE-CLICK WINDOWS LAUNCHER ---
# Workspace root folder (d:\Alumni_system) mein command prompt kholo aur run karo:
run_all.bat
# Ye backend ko port 8080 aur frontend ko port 3000 par automatically start kar dega!

# --- OPTION B: MANUAL STEP-BY-STEP RUN ---
# 1. C++ Backend Build & Start:
cd backend-cpp/build
cmake ..
cmake --build .
./alumniverse_backend.exe
# Terminal par dikhega: [INFO] Binding AlumniVerse HTTP Server to 0.0.0.0:8080

# 2. React Frontend Start (in a separate terminal):
cd frontend
npm install
npm run dev
# Browser mein open karein: http://localhost:5173 ya http://localhost:3000

# --- OPTION C: FULL DOCKER COMPOSE RUN ---
docker-compose up --build
# Starts Postgres (5432), C++ Backend (8080), React Frontend (3000)
`


## 9.3 Actual Test Results (Verified Locally)

Aapke project ke tests ko actual terminal par run karke verify kiya gaya hai. Ye actual test results hain jinhe aap sir ke saamne confidently dikha sakte hain:


| Test Suite / Verification Task | Command Executed | Actual Result | Status |
| --- | --- | --- | --- |
| C++ Graph, Tree & Security Unit Tests | .\backend-cpp\build\graph_tests.exe | Passed: 50, Failed: 0. Covers empty graph, cyclic traversal, shortest path, tree pruning, password hashing, JWT, CSV sanitization. | [TESTED & PASSED] |
| Frontend Linter (Oxlint) | npm run lint (in frontend/) | Found 29 warnings, 0 errors. Passed in 104ms on 18 files. | [TESTED & PASSED] |
| Frontend Production Build (Vite) | npm run build (in frontend/) | Built in 1.07s. index.html (1.61 kB), CSS (81 kB), JS bundle (607 kB) generated in frontend/dist/. | [TESTED & PASSED] |



## 9.4 Production Deployment Checklist & Security Concerns


## 1. CORS Security: CORS Origins: Abhi main.cpp:22 mein Access-Control-Allow-Origin: * set hai taaki local testing easy rahe. Production mein isse specific domain (jaise https://alumniverse.iips.edu) par restrict karna chahiye.


## 2. JWT Secret: JWT Secret Key: .env file mein JWT_SECRET ko default value se badal kar ek strong 64-character random string set karein.


## 3. Database Persistence: PostgreSQL Volume Persistence: docker-compose.yml mein postgres_data volume configured hai jisse container restart hone par bhi data delete nahi hota.


## 4. HTTPS: HTTPS SSL Certificate: Free hosting platforms (jaise Render ya Vercel) automatic SSL certificate provide karte hain.


---

# PART 10  -  Viva & Sir Ko Explain Karne Ki Full Preparation


## 10.1 2-Minute Elevator Pitch (Sir Ko Quick Samjhane Ke Liye)

> [!NOTE]
> **[2-MINUTE ELEVATOR PITCH (MEMORIZE THIS!)] "Good morning Sir! Humara project hai AlumniVerse  -  Interactive Alumni Tree & Graph Intelligence Platform. College alumni directories aksar flat Excel sheets ya PDFs hoti hain jisme students ko network structure aur connections samajh nahi aate. Humne is problem ko solve karne ke liye ek 3-Tier architecture banaya hai: Frontend modern React 19 aur @xyflow/react par chal raha hai jo academic hierarchy (Institution -> Department -> Batch -> Alumni) aur professional relationship graph ko visually render karta hai. Project ka main core ek high-performance C++17 Crow backend engine hai. C++ use karne ka sabse bada reason hai large-scale graph traversals: humne Adjacency List representation par BFS aur DFS traversal implement kiya hai, aur unweighted BFS parent-tracking se do alumni ke beech ka Shortest Connection Path O(V + E) time complexity mein calculate hota hai. Saath hi backend mein salted SHA-256 password hashing, pure C++ JWT generation, aur formula injection protected CSV bulk import engine hai. Humare 50 ke 50 unit tests successfully pass ho chuke hain!"**


## 10.2 3–5 Minute Detailed Presentation Script

Agar presentation mein 5 minutes milein, toh slide-by-slide is flow mein bolein:

- Slide 1: Problem & Motivation: "Hello everyone. Today we are presenting AlumniVerse. Traditional college directories fail to answer two critical questions: 1) How does our alumni body fit into the academic and industrial hierarchy? 2) How can a current student reach an alumnus in a target company like Google or Microsoft through existing alumni mutual connections?"
- Slide 2: System Architecture: "To solve this, we decoupled our system into 3 layers: A responsive React 19 Frontend for canvas-based visualization, a C++17 Crow REST API Engine for high-speed graph computations, and a structured PostgreSQL database schema with 11 relational tables."
- Slide 3: Core Algorithms & C++ Engine: "In our C++ backend, we designed two primary data structures: First, AlumniTree which organizes academic and industry hierarchies. It features recursive bottom-up subtree pruning: when a user filters by a skill like 'Distributed Systems', branches with 0 matches are pruned dynamically while ancestor paths are preserved. Second, AlumniGraph which models professional relationships using an adjacency list. We implemented cycle-safe BFS, DFS, and an unweighted BFS Shortest Path algorithm that finds the shortest degree of separation between any two alumni in O(V + E) time."
- Slide 4: Security & Validation: "Security is embedded natively in C++: we implemented FIPS 180-2 SHA-256 with random 12-character salt and constant-time verification to prevent timing attacks. We also created a pure C++ JWT HMAC-SHA256 token generator, and our CSV parser sanitizes formula injection attacks. All our core modules are backed by a comprehensive unit test suite in GraphTests.cpp with 50 passing test assertions."
- Slide 5: Live Flow & Conclusion: "Let's demonstrate a live trace: When an admin adds a new alumnus, the request hits our Crow router, validates the JWT token, inserts the record into DataRepository with mutex thread-safety, refreshes the in-memory graph cache, and the React Flow canvas instantly renders the new node with computed coordinates. Thank you, and we welcome your questions!"

## 10.3 Top 25 Viva Questions & Short Code-Verified Answers


#### Q1: Backend ke liye C++ kyun choose kiya? Node.js ya Python kyun nahi?

Answer: Graph algorithms (BFS, DFS, Shortest Path) aur tree pruning CPU-intensive hote hain. C++17 zero-overhead abstractions, direct memory control, aur high-speed execution provide karta hai. nlohmann/json aur Crow framework ke saath humne REST API banayi hai jo lightweight aur super-fast hai.


#### Q2: Crow framework kya hai aur ye kaise kaam karta hai?

Answer: Crow C++ ka ek lightweight, header-only web microframework hai (inspired by Python Flask / Express). Ye Asio library par based hai aur multithreaded HTTP request handling aur route dispatching provide karta hai (src/main.cpp:107).


#### Q3: Graph ko C++ mein kaise represent kiya gaya hai?

Answer: Adjacency List se represent kiya gaya hai using std::unordered_map<int, std::vector<GraphEdge>> adjList_ (AlumniGraph.hpp:164). Adjacency Matrix ke mukable ye sparse graphs mein space save karta hai (O(V + E) space instead of O(V^2)).


#### Q4: Graph directed hai ya undirected?

Answer: Undirected hai. Jab do alumni ke beech connection banta hai, toh addEdge() dono vertices ke adjacency list mein edge push karta hai: adjList_[sourceId].push_back(...) aur adjList_[targetId].push_back(...) (AlumniGraph.cpp:59-60).


#### Q5: BFS aur DFS mein kya difference hai aur code mein kahan hain?

Answer: BFS (Breadth-First Search) level-by-level explore karta hai using FIFO queue (std::queue), shortest path dhoondhne ke liye best hai. DFS (Depth-First Search) branch ki gehraai tak jaata hai using LIFO stack (std::stack). Dono AlumniGraph.cpp lines 95 aur 128 par implemented hain.


#### Q6: Graph mein cycle ho toh BFS infinite loop mein kyun nahi fasta?

Answer: Kyunki humne std::unordered_set<int> visited maintain kiya hai. Queue mein push karne se pehle check hota hai if (visited.find(id) == visited.end()). Jo node ek baar visit ho gaya wo dubara queue mein nahi jaata (AlumniGraph.cpp:117).


#### Q7: Shortest Path algorithm kaise kaam karta hai? Dijkstra kyun nahi use kiya?

Answer: Hamara graph unweighted hai (har connection = 1 hop). Unweighted graphs mein BFS guaranteed shortest path O(V + E) time mein deta hai, jabki Dijkstra O(E log V) leta. Humne parent-map tracking use karke target se source tak backtrack kiya aur reverse karke path return kiya (AlumniGraph.cpp:162).


#### Q8: Tree aur Graph mein kya farak hai?

Answer: Tree ek hierarchical structure hai jisme strictly 1 root hota hai, 0 cycles hoti hain, aur N nodes ke N-1 edges hote hain. Graph ek general network hai jisme cycles ho sakti hain aur multiple components ho sakte hain.


#### Q9: Tree pruning (filterTree) kaise kaam karta hai?

Answer: filterSubtreeRecursive() post-order traversal karta hai. Agar leaf alumni filter se match nahi karta toh nullptr return hota hai. Agar kisi branch ke saare children null ho jayein, toh parent branch bhi prune ho jaati hai, lekin matching alumni ke ancestor hierarchy paths preserve rehte hain (AlumniTree.cpp:315).


#### Q10: React Flow canvas par nodes overlap kyun nahi hote?

Answer: C++ backend ka exportToReactFlowJson() function level-by-level BFS queue chalaata hai. Har level ke nodes count karke math formula (posX = startX + i * 260, posY = 80 + lvl * 160) se exact coordinates assign karta hai (AlumniTree.cpp:394).


#### Q11: Multithreaded server mein race conditions kaise roki hain?

Answer: DataRepository mein mutable std::mutex mtx_ use kiya hai. Har write/read operation par std::lock_guard<std::mutex> lock(mtx_) use hota hai jo RAII pattern ke through automatic locking aur unlocking ensure karta hai (DataRepository.cpp:519).


#### Q12: Passwords kaise store hote hain? Plaintext ya Hash?

Answer: Salted SHA-256 hash format mein: salt$hash. Har user ke liye 12-char random alphanumeric salt banta hai taaki rainbow table attacks fail ho jayein (CryptoUtils.hpp:128).


#### Q13: Constant-time password verification kya hota hai aur kyun zaroori hai?

Answer: Agar hum strcmp ya == use karein toh CPU pehle mismatch character par ruk jaata hai, jisse attacker response time measure karke characters guess kar sakta hai (timing attack). Humne bitwise OR (diff |= actual[i] ^ expected[i]) use kiya hai jo har baar exact constant time leta hai (CryptoUtils.hpp:148).


#### Q14: JWT token kaise generate aur verify hota hai?

Answer: Pure C++ mein HMAC-SHA256 signature se. Header aur payload ko Base64Url encode karke secret key ke saath sign kiya jaata hai. verifyJwt() signature match karta hai aur exp (timestamp) check karke token validate karta hai (CryptoUtils.hpp:207, 230).


#### Q15: CSV import mein Formula Injection (DDE attack) kya hota hai?

Answer: Agar koi user CSV mein full_name ki jagah '=cmd|...' daal de, toh Excel kholne par malicious program run ho sakta hai. Humara sanitizeCell() function check karta hai: agar cell '=', '+', '-', '@' se shuru ho, toh wo aage single quote (') laga kar neutralise kar deta hai (CsvUtils.hpp:36).


#### Q16: React Flow mein custom nodes kaise banaye hain?

Answer: CustomNodes.jsx mein AlumniNode aur HierarchyNode components banaye hain. React Flow ke Handle component (type='target' aur type='source') se edges connect hoti hain (CustomNodes.jsx:11, 88).


#### Q17: Skills filter mein OR aur AND logic ka kya matlab hai?

Answer: OR logic: Alumni ke paas selected skills mein se koi bhi ek ho (e.g. C++ ya Java). AND logic: Alumni ke paas saari selected skills hona mandatory hai (e.g. C++ aur Distributed Systems dono) (AlumniTree.cpp:270-297).


#### Q18: Database schema mein partial unique index kyun lagaya hai?

Answer: career_history table par uq_alumni_single_current_job index lagaya hai WHERE is_current = TRUE. Isse database level par guarantee hoti hai ki ek alumni ka simultaneously ek se zyada current job nahi ho sakta (001_initial_schema.sql:108).


#### Q19: CORS kya hai aur backend ne isse kaise handle kiya?

Answer: Cross-Origin Resource Sharing. Browser port 3000 (React) se port 8080 (C++) par request bhejte time block na kare, isliye main.cpp:21 mein setCorsHeaders() helper Access-Control-Allow-Origin: * aur OPTIONS preflight (204 No Content) set karta hai.


#### Q20: Audit logs kya hain aur kya track karte hain?

Answer: System security logs. Jab bhi koi alumni create, update, delete, ya verify hota hai, DataRepository::logAction() actor ka user ID, name, action, aur timestamp audit_logs table mein record karta hai (main.cpp:282).


#### Q21: Unit tests kaise run hote hain aur kitne pass hue?

Answer: backend-cpp/tests/GraphTests.cpp mein test suite hai jo graph_tests.exe ke roop mein compile hota hai. Humne run kiya aur 50/50 test assertions PASSED hain, 0 FAILED.


#### Q22: Frontend build tool kaunsa hai?

Answer: Vite v8.3.4. Ye standard Webpack se 10x-50x fast hai. Production bundle npm run build se 1.07 second mein build ho jaata hai.


#### Q23: Frontend API client mein interceptors ka kya kaam hai?

Answer: src/api/client.js mein request interceptor localStorage se token nikaal kar har outgoing HTTP request mein Authorization: Bearer <jwt> header automatically attach karta hai (client.js:13).


#### Q24: Degree of separation ka kya matlab hai?

Answer: Do alumni ke beech kitne connections ka fasla hai. Agar Karan direct Priya ko jaanta hai toh degree = 1. Agar Karan -> Priya -> Aarav hai toh degree = 2.


#### Q25: Project ko production mein host karne ke liye kya karna hoga?

Answer: docker-compose.yml ya render.yaml use kar sakte hain. Production mein CORS origin ko specific domain par lock karenge, JWT secret ko strong random key banayenge, aur Nginx reverse proxy se SSL/HTTPS terminate karenge.


## 10.4 'Agar Sir Poochhe Ye Code Kahan Hai?' Quick Mapping Table

Sir ke bolte hi bina confuse hue seedha code file kholne ke liye ye cheatsheet table use karein:


| Examiner Question: 'Ye Kahan Likha Hai?' | Exact File Path | Relevant Function / Struct / Symbol | Line Range |
| --- | --- | --- | --- |
| C++ Server Startup & Port Binding | backend-cpp/src/main.cpp | main(), app.bindaddr().port().run() | Lines 96-613 |
| CORS Header Setup | backend-cpp/src/main.cpp | setCorsHeaders(), OPTIONS route | Lines 21-26, 110-123 |
| Breadth-First Search (BFS) | backend-cpp/src/graph/AlumniGraph.cpp | AlumniGraph::bfs() | Lines 95-126 |
| Depth-First Search (DFS) | backend-cpp/src/graph/AlumniGraph.cpp | AlumniGraph::dfs() | Lines 128-160 |
| Shortest Path Discovery | backend-cpp/src/graph/AlumniGraph.cpp | AlumniGraph::findShortestPath() | Lines 162-207 |
| Graph Edge Insertion & Cycle/Self Loop Check | backend-cpp/src/graph/AlumniGraph.cpp | AlumniGraph::addEdge() | Lines 46-63 |
| Academic Tree Hierarchy Building | backend-cpp/src/graph/AlumniTree.cpp | AlumniTree::buildAcademicTree() | Lines 23-93 |
| Tree Subtree Pruning Logic | backend-cpp/src/graph/AlumniTree.cpp | AlumniTree::filterSubtreeRecursive() | Lines 315-341 |
| React Flow Coordinate Math (BFS layout) | backend-cpp/src/graph/AlumniTree.cpp | AlumniTree::exportToReactFlowJson() | Lines 348-440 |
| Circular Graph Layout Math | backend-cpp/src/graph/AlumniGraph.cpp | AlumniGraph::getInducedSubgraphJson() | Lines 225-287 |
| Salted Password Hash & Constant Time Verify | backend-cpp/include/utils/CryptoUtils.hpp | hashPassword(), verifyPassword() | Lines 128-153 |
| Pure C++ JWT Generator & Validator | backend-cpp/include/utils/CryptoUtils.hpp | generateJwt(), verifyJwt() | Lines 207-258 |
| CSV Formula Injection Sanitizer | backend-cpp/include/utils/CsvUtils.hpp | CsvUtils::sanitizeCell() | Lines 31-41 |
| Mutex Lock Guard Thread Safety | backend-cpp/src/repositories/DataRepository.cpp | std::lock_guard<std::mutex> lock(mtx_) | Lines 510, 519, 547 |
| Data Model Structs (User, Alumni, Batch) | backend-cpp/include/models/Models.hpp | struct Alumni, Batch, User, FilterCriteria | Lines 31-318 |
| React Flow Canvas Integration | frontend/src/pages/ExplorerPage.jsx | ExplorerPage, loadVisualizationData() | Lines 30-117 |
| Custom Canvas Nodes (AlumniNode) | frontend/src/components/CustomNodes.jsx | AlumniNode, HierarchyNode, nodeTypes | Lines 11-167 |
| Axios Interceptor for JWT | frontend/src/api/client.js | api.interceptors.request.use | Lines 13-19 |
| SQL Schema & Partial Unique Index | database/migrations/001_initial_schema.sql | CREATE TABLE, uq_alumni_single_current_job | Lines 8-159 |



---

# PART 11  -  Learn The Code Yourself & Final Mastery Checklist


## 11.1 Beginner-Friendly 7-Step Learning Roadmap

Agar aapko code samajhne mein darr lagta hai, toh is exact sequence mein files ko padhein:

Padhne Ka Sahi Sequence: Step 1: backend-cpp/include/models/Models.hpp kholo. Sirf structs dekho ki User kya hai aur Alumni kya hai.

Padhne Ka Sahi Sequence: Step 2: backend-cpp/src/main.cpp dekho. Sirf main() function aur CROW_ROUTE dekho ki request kahan aati hai.

Padhne Ka Sahi Sequence: Step 3: backend-cpp/src/graph/AlumniGraph.cpp kholo. addVertex(), addEdge(), aur bfs() dekho.

Padhne Ka Sahi Sequence: Step 4: backend-cpp/src/graph/AlumniTree.cpp kholo. buildAcademicTree() aur filterSubtreeRecursive() dekho.

Padhne Ka Sahi Sequence: Step 5: backend-cpp/tests/GraphTests.cpp chalao. Dekho ki tests kaise assertions check karte hain.

Padhne Ka Sahi Sequence: Step 6: frontend/src/api/client.js dekho ki frontend kaise C++ backend ko call karta hai.

Padhne Ka Sahi Sequence: Step 7: frontend/src/pages/ExplorerPage.jsx dekho ki nodes canvas par kaise aate hain.


## 11.2 WhatsApp Hinglish Technical Glossary


| Technical Term | Asli Matlab (Simple WhatsApp Hinglish) | Real-Life Analogy |
| --- | --- | --- |
| Adjacency List | Ek aisi list jisme har alumni ke samne unke sabhi dosto ke naam likhe hote hain. | Phone ki contacts list jisme har bande ke aage uske friends ka group ho. |
| BFS (Breadth-First) | Pehle nazdeek ke saare dosto ko dekhna, phir unke dosto ko dekhna (choudaai mein aage badhna). | Paani mein pathar maarne par banne wali gol waves. |
| DFS (Depth-First) | Ek hi raaste par aakhiri dum tak jaate rehna jab tak dead-end na aa jaye, phir wapas aana. | Bhool-bhulaiya (maze) mein ek diwaar pakad kar chalte jaana. |
| Pruning | Tree ki un branches ko kaat kar hata dena jisme matching data nahi hai. | Bagiche ke ped ki sukhi daaliyon ko kaat kar saaf karna. |
| Mutex (Mutual Exclusion) | Ek lock jo ensure karta hai ki ek time par sirf ek thread data ko edit kare. | Single-person trial room jisme lock laga kar kapde change karte hain. |
| Salted Hash | Password ke peeche random kachra jod kar code mein badalna taaki koi reverse na kar sake. | Doodh mein bournvita mila kar ghol dena; ab doodh aur bournvita alag nahi ho sakte. |
| Constant-Time Verification | Har check mein barabar time lena taaki hacker time dekh kar guess na kar sake. | Aap chahe sahi password dalo ya galat, gatekeeper hamesha 5 second baad hi gate kholega. |
| JWT Token | Ek signed digital pass jo user ke jeb (localStorage) mein rehta hai taaki bar-bar password na mangna pade. | Metro train ka smart card jisko tap karke har station par entry milti hai. |
| CORS Preflight (OPTIONS) | Browser ka pehle se poochhna: 'Kya main doosre port se data mangwa sakta hoon?' | Ghar mein ghusne se pehle doorbell baja kar poochhna ki 'main andar aa jaun?' |
| Induced Subgraph | Bade graph mein se sirf selected nodes aur unke aapas ke connecting wires ka chhota tukda. | World map mein se sirf Indore aur Bhopal ke beech ki road ka cutout nikaalna. |



## 11.3 Project Ownership Final Checklist

Examiner ke saamne jaane se pehle check karein ki kya aap in sabhi cheezon mein confident hain:

- Checklist Item 1: Kya aap 2-minute elevator pitch bina ruke bol sakte hain? [YES / NO]
- Checklist Item 2: Kya aap graph_tests.exe terminal par chala kar 50 passing tests dikha sakte hain? [YES / NO]
- Checklist Item 3: Kya aap batate hain ki DataRepository thread-safe mutex in-memory engine hai with PostgreSQL schema parity? [YES / NO]
- Checklist Item 4: Kya aap BFS Shortest Path algorithm ka queue aur parent map dry-run whiteboard par bana sakte hain? [YES / NO]
- Checklist Item 5: Kya aap Tree pruning (filterSubtreeRecursive) ka logic samjha sakte hain? [YES / NO]
- Checklist Item 6: Kya aap constant-time verification aur salted password hashing ka reason de sakte hain? [YES / NO]
> [!NOTE]
> **[FINAL CONFIDENCE BOOSTER] Agar aapne upar diye gaye sabhi checklist items ko prepare kar liya hai, toh duniya ka koi bhi examiner ya professor aapko viva mein confuse nahi kar sakta! Aap is project ke complete owner aur master ban chuke hain. ALL THE VERY BEST FOR YOUR VIVA!**
