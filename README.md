# AlumniVerse — Interactive Alumni Tree & Graph Intelligence Platform

**AlumniVerse** is a production-minded, high-performance alumni network web platform engineered for universities and collegiate institutions (configured out-of-the-box for **IIPS DAVV**, and fully customizable for any academic body).

The platform pairs an authoritative **C++17 Crow microservices backend** implementing core graph and tree algorithms (BFS, DFS, shortest path discovery, subtree filter pruning) with an interactive **React Flow (@xyflow/react)** visual canvas and a normalized **PostgreSQL** data model.

---

## 1. System Architecture

```
                      +---------------------------------------+
                      |       React 19 Frontend (Vite)        |
                      |  - Interactive React Flow DAG Canvas  |
                      |  - Subtree Filter Sidebar             |
                      |  - Dark Mode Glassmorphism Theme      |
                      +-------------------+-------------------+
                                          |
                                          | HTTPS / REST (JSON)
                                          | Port 8080 (or /api/v1)
                                          v
                      +---------------------------------------+
                      |       C++17 Crow REST Backend         |
                      |  - Adjacency List (AlumniGraph)       |
                      |  - Hierarchical Tree (AlumniTree)     |
                      |  - Unweighted BFS Shortest Path       |
                      |  - DFS & Subtree Pruning Engine       |
                      |  - Argon2/SHA-256 HMAC & JWT Auth     |
                      |  - Audit Log & Formula Injection San. |
                      +-------------------+-------------------+
                                          |
                                          | SQL / DATABASE_URL
                                          v
                      +---------------------------------------+
                      |         PostgreSQL 16 Engine          |
                      |  - 11 Normalized Relational Tables    |
                      |  - Partial Unique Constraint Indexes  |
                      |  - Cascade & Foreign Key Governance   |
                      +---------------------------------------+
```

---

## 2. Algorithmic Specifications & Time Complexity

All graph traversals and tree operations are executed natively within the compiled C++ engine:

| Operation | Data Structure / Method | Time Complexity | Space Complexity | Description |
|---|---|---|---|---|
| **Graph Insertion** | `std::unordered_map` | $\mathcal{O}(1)$ avg | $\mathcal{O}(V + E)$ | Inserts vertices and bidirectional relationship edges. |
| **Duplicate Edge Check** | Vector inspection | $\mathcal{O}(\text{deg}(u))$ | $\mathcal{O}(1)$ | Prevents duplicate edges and self-loops. |
| **Shortest Path** | Unweighted BFS | $\mathcal{O}(V + E)$ | $\mathcal{O}(V)$ | Traverses adjacency list with parent-pointer reconstruction to discover optimal degrees of separation. |
| **Cycle-Safe BFS** | Queue + Visited Set | $\mathcal{O}(V + E)$ | $\mathcal{O}(V)$ | Terminates deterministically on cyclic network graphs without infinite recursion. |
| **Cycle-Safe DFS** | Stack + Visited Set | $\mathcal{O}(V + E)$ | $\mathcal{O}(V)$ | Performs deep network walks with configurable depth caps. |
| **Subtree Pruning** | Post-order Tree Walk | $\mathcal{O}(N)$ | $\mathcal{O}(H)$ | Prunes leaves/branches with zero filter matches while preserving ancestral paths down to matching alumni. |
| **React Flow Layout** | Level-order BFS | $\mathcal{O}(N)$ | $\mathcal{O}(N)$ | Computes horizontal span and vertical level offsets $(X, Y)$ for direct rendering. |

---

## 3. Database Schema Overview

The database is normalized across 11 core tables (`database/migrations/001_initial_schema.sql`):

1. **`users`**: Authentication credentials, roles (`admin`, `alumni`, `viewer`), and timestamps.
2. **`institutions`**: Academic universities/institutions (e.g. IIPS DAVV).
3. **`departments`**: Academic faculties (e.g. Computer Applications, Management Studies).
4. **`batches`**: Academic degree cohorts with start and graduation years.
5. **`alumni`**: Core alumni profile entities linked to department and batch.
6. **`companies`**: Industry employers and sectors.
7. **`career_history`**: Employment records with a **partial unique constraint** enforcing at most one current position:
   ```sql
   CREATE UNIQUE INDEX uq_alumni_single_current_job 
   ON career_history(alumni_id) 
   WHERE is_current = TRUE;
   ```
8. **`skills`**: Normalized skill tags (e.g. C++, Distributed Systems, System Design).
9. **`alumni_skills`**: Composite primary key junction table.
10. **`alumni_connections`**: Confirmed professional edges (`classmate`, `colleague`, `mentor`, `professional_connection`) with self-loop rejection constraints.
11. **`audit_logs`**: Administrative activity audit logs tracking actor, action, entity, and timestamp.

---

## 4. REST API Documentation (`/api/v1`)

### Health & Analytics
- `GET /health` / `GET /api/v1/health`: Service health and database status.
- `GET /api/v1/admin/analytics`: Real-time aggregated metrics (total alumni, verified count, companies represented, graduation year distribution).

### Authentication
- `POST /api/v1/auth/register`: Create a new user account with salted password hashing.
- `POST /api/v1/auth/login`: Authenticate and receive a signed JWT token.
- `GET /api/v1/auth/me`: Retrieve current user identity and role (Bearer token required).

### Tree Hierarchy
- `GET /api/v1/tree`: Full hierarchical tree in React Flow format. Query parameters:
  - `hierarchy`: `academic` (default) or `industry`.
  - Filter parameters: `search`, `batch_id`, `company`, `industry`, `skills`, `skills_logic` (`and` / `or`), `verification_status`.
- `GET /api/v1/tree/:nodeId/subtree`: Isolate specific branch (e.g., `batch-1`, `dept-1`) and its descendants.

### Relationship Graph
- `GET /api/v1/graph`: Full adjacency network or filtered induced subgraph.
- `GET /api/v1/graph/:id/neighbors`: Immediate confirmed connections and details.
- `GET /api/v1/graph/path?source=:src&target=:tgt`: C++ BFS shortest path discovery between two alumni IDs.

### Alumni Directory
- `GET /api/v1/alumni`: Paginated alumni listing with multi-field search and filters.
- `GET /api/v1/alumni/:id`: Complete dossier including career history and confirmed connections.
- `POST /api/v1/alumni`: Create new alumni record (Admin/Alumni authorization).
- `PUT /api/v1/alumni/:id`: Update existing alumni record.
- `DELETE /api/v1/alumni/:id`: Delete alumni record (Admin only).

### Administration
- `POST /api/v1/admin/alumni/:id/verify`: Approve or update profile verification status.
- `POST /api/v1/admin/import/csv`: Bulk CSV import with formula injection sanitization.
- `POST /api/v1/admin/connections`: Establish confirmed graph edge.
- `GET /api/v1/admin/audit-logs`: Inspect chronological audit trail.

---

## 5. Project Directory Tree

```
Alumni_system/
├── backend-cpp/
│   ├── CMakeLists.txt
│   ├── Dockerfile
│   ├── include/
│   │   ├── controllers/
│   │   ├── graph/
│   │   │   ├── AlumniGraph.hpp
│   │   │   └── AlumniTree.hpp
│   │   ├── models/
│   │   │   └── Models.hpp
│   │   ├── repositories/
│   │   │   └── DataRepository.hpp
│   │   ├── services/
│   │   │   ├── AuthService.hpp
│   │   │   ├── GraphService.hpp
│   │   │   └── TreeService.hpp
│   │   ├── utils/
│   │   │   ├── CryptoUtils.hpp
│   │   │   └── CsvUtils.hpp
│   │   └── vendor/
│   │       ├── asio.hpp & asio/
│   │       ├── crow_all.h
│   │       └── nlohmann/json.hpp
│   ├── src/
│   │   ├── graph/
│   │   │   ├── AlumniGraph.cpp
│   │   │   └── AlumniTree.cpp
│   │   ├── repositories/
│   │   │   └── DataRepository.cpp
│   │   ├── services/
│   │   │   ├── AuthService.cpp
│   │   │   ├── GraphService.cpp
│   │   │   └── TreeService.cpp
│   │   └── main.cpp
│   └── tests/
│       └── GraphTests.cpp
├── database/
│   ├── migrations/
│   │   └── 001_initial_schema.sql
│   └── seeds/
│       ├── 001_seed_data.sql
│       └── sample_alumni_import.csv
├── frontend/
│   ├── Dockerfile
│   ├── index.html
│   ├── package.json
│   ├── vite.config.js
│   └── src/
│       ├── api/
│       │   └── client.js
│       ├── components/
│       │   ├── AlumniDetailPanel.jsx
│       │   ├── CustomNodes.jsx
│       │   ├── FilterSidebar.jsx
│       │   ├── Footer.jsx
│       │   ├── Navbar.jsx
│       │   └── ShortestPathModal.jsx
│       ├── context/
│       │   └── AuthContext.jsx
│       ├── pages/
│       │   ├── AdminPage.jsx
│       │   ├── DashboardPage.jsx
│       │   ├── DirectoryPage.jsx
│       │   ├── ExplorerPage.jsx
│       │   ├── LandingPage.jsx
│       │   ├── LoginPage.jsx
│       │   └── ProfileDetailPage.jsx
│       ├── App.jsx
│       ├── index.css
│       └── main.jsx
├── docker-compose.yml
├── render.yaml
├── .env.example
├── .gitignore
└── README.md
```

---

## 6. Local Setup and Run Instructions

### Option A: Local Development (Windows / Linux)

#### 1. Compile and Run the C++ Backend
```powershell
cd backend-cpp
cmake -B build -G Ninja
ninja -C build alumniverse_backend
./build/alumniverse_backend.exe
```
The backend initializes on `http://0.0.0.0:8080`.

#### 2. Run the C++ Unit Tests
```powershell
ninja -C build graph_tests
./build/graph_tests.exe
```

#### 3. Run the React Frontend
```powershell
cd ../frontend
npm install
npm run dev
```
Open `http://127.0.0.1:5173/` in your browser.

---

### Option B: Docker Compose (All Services with PostgreSQL)

```bash
docker compose up --build
```
This starts:
- `postgres` on port `5432` with initial migrations and seed records.
- `alumniverse_backend` on port `8080`.
- `alumniverse_frontend` on port `3000`.

---

## 7. Automated Test Results

The C++ unit test suite (`backend-cpp/tests/GraphTests.cpp`) was executed natively:

```
========================================================
  AlumniVerse C++ Graph, Tree & Security Engine Tests  
========================================================

--- Testing Empty Graph ---
[+] PASSED: Vertex count is 0 on empty graph
[+] PASSED: Edge count is 0 on empty graph
[+] PASSED: hasVertex returns false for non-existent node
[+] PASSED: BFS returns empty vector on non-existent node
[+] PASSED: DFS returns empty vector on non-existent node
[+] PASSED: findShortestPath returns empty vector on empty graph

--- Testing Single Vertex ---
[+] PASSED: Vertex count is 1 after addition
[+] PASSED: hasVertex(101) is true
[+] PASSED: getVertex returns correct node data
[+] PASSED: BFS on single vertex returns [101]
[+] PASSED: DFS on single vertex returns [101]
[+] PASSED: Path from vertex to itself returns [101]
[+] PASSED: removeVertex succeeds
[+] PASSED: Vertex count is 0 after removal
[+] PASSED: hasVertex is false after removal

--- Testing Edge Validation & Duplicate Handling ---
[+] PASSED: Cannot add edge to non-existent vertex 999
[+] PASSED: Self loop edge rejected
[+] PASSED: Valid edge (1 <-> 2) added successfully
[+] PASSED: Edge count is 1
[+] PASSED: Duplicate edge correctly rejected
[+] PASSED: Edge count remains 1 after duplicate rejection
[+] PASSED: Vertex 1 has neighbor 2
[+] PASSED: Vertex 2 has neighbor 1

--- Testing Disconnected Components ---
[+] PASSED: BFS from node 1 visits exactly 3 nodes in component 1
[+] PASSED: BFS from node 4 visits exactly 2 nodes in component 2
[+] PASSED: Shortest path between disconnected components is empty

--- Testing Cyclic Traversal & Shortest Path ---
[+] PASSED: BFS terminates safely on cyclic graph and visits all 5 nodes
[+] PASSED: DFS terminates safely on cyclic graph and visits all 5 nodes
[+] PASSED: Shortest path 1 -> 5 has 3 nodes (length 2 edges)
[+] PASSED: Shortest path correctly chooses direct chord [1 -> 3 -> 5]

--- Testing Hierarchical Tree & Subtree Pruning ---
[+] PASSED: Tree root is generated
[+] PASSED: Root alumni count is 3
[+] PASSED: Subtree dept-1 found
[+] PASSED: dept-1 has 2 alumni
[+] PASSED: Filtered tree exists
[+] PASSED: Filtered tree contains 1 matching alumni (Google)
[+] PASSED: Filtering with 0 matches prunes entire tree cleanly to null

--- Testing Password Hashing & JWT Security ---
[+] PASSED: Password hash generated
[+] PASSED: Hash format contains salt delimiter ($)
[+] PASSED: Password verification succeeds with correct password
[+] PASSED: Password verification fails with incorrect password
[+] PASSED: JWT token generated
[+] PASSED: Valid JWT verified successfully
[+] PASSED: JWT payload contains correct userId
[+] PASSED: JWT payload contains correct role
[+] PASSED: JWT with wrong secret key fails verification

--- Testing CSV Parser & Formula Injection Protection ---
[+] PASSED: Parsed 2 data rows
[+] PASSED: First row name parsed correctly
[+] PASSED: Quoted comma-separated skills parsed
[+] PASSED: Formula injection cell beginning with '=' was safely sanitized with leading quote

========================================================
TEST RESULTS SUMMARY:
Passed: 50
Failed: 0
========================================================
```

---

## 8. Render Deployment Instructions

The project includes a ready-to-deploy **Render Blueprint** (`render.yaml`).

### Deployment Steps:
1. Push your repository to GitHub or GitLab.
2. Log in to [Render Dashboard](https://dashboard.render.com).
3. Click **New +** &rarr; **Blueprint**.
4. Connect the repository and select `render.yaml`.
5. Render will automatically provision:
   - **PostgreSQL Database** (`alumniverse-postgres`)
   - **C++ Web Service** (`alumniverse-backend-cpp`) using `backend-cpp/Dockerfile`
   - **Static Site** (`alumniverse-frontend`) with SPA rewrite rules
6. Under the PostgreSQL dashboard in Render, execute `database/migrations/001_initial_schema.sql` and `database/seeds/001_seed_data.sql` using the provided connection string.

---

## 9. Demo Credentials

| Role | Email | Password | Permissions |
|---|---|---|---|
| **System Admin** | `admin@alumniverse.edu` | `Password123!` | Full governance, verification, CSV import, audit logs |
| **Alumni Member** | `aarav.sharma@alumniverse.edu` | `Password123!` | Edit profile, manage connections, view explorer |
| **Viewer** | `viewer@alumniverse.edu` | `Password123!` | Read-only directory and network explorer browsing |

---

## 10. Security and Privacy Safeguards

- **No LinkedIn Scraping**: Profile URLs are user-submitted or administrator-verified. External links open with `rel="noopener noreferrer" target="_blank"`.
- **Constant-Time Verification**: Password verification uses bitwise XOR comparisons to eliminate timing side-channels.
- **CSV Formula Injection Mitigation**: Any CSV cells prefixed with `=`, `+`, `-`, or `@` are neutralized with safe leading quote sanitization.
- **Role-Based Access Control**: Administrative actions and deletions require valid JWT signatures with `role: "admin"`.
