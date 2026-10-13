-- AlumniVerse Seed Data
-- Fictional seed data for academic institution: IIPS DAVV (Indore, MP, India)

-- Clean tables before seeding (for reproducible runs)
TRUNCATE TABLE audit_logs CASCADE;
TRUNCATE TABLE alumni_connections CASCADE;
TRUNCATE TABLE alumni_skills CASCADE;
TRUNCATE TABLE skills CASCADE;
TRUNCATE TABLE career_history CASCADE;
TRUNCATE TABLE companies CASCADE;
TRUNCATE TABLE alumni CASCADE;
TRUNCATE TABLE batches CASCADE;
TRUNCATE TABLE departments CASCADE;
TRUNCATE TABLE institutions CASCADE;
TRUNCATE TABLE users CASCADE;

-- 1. USERS
-- Password for all seed users is: Password123! (hashed using SHA256-salt: $salt$hash)
-- Salt: s3cr3tsalt, Hash of "Password123!s3cr3tsalt": e654e8ec67bf75fc3f22d9bfe6cd311dc2719c8d197607a97ea9fa82eb068307
-- Stored as: salt$hash format
INSERT INTO users (id, name, email, password_hash, role) VALUES
(1, 'System Administrator', 'admin@alumniverse.edu', 's3cr3tsalt$e654e8ec67bf75fc3f22d9bfe6cd311dc2719c8d197607a97ea9fa82eb068307', 'admin'),
(2, 'Aarav Sharma', 'aarav.sharma@alumniverse.edu', 's3cr3tsalt$e654e8ec67bf75fc3f22d9bfe6cd311dc2719c8d197607a97ea9fa82eb068307', 'alumni'),
(3, 'Priya Patel', 'priya.patel@alumniverse.edu', 's3cr3tsalt$e654e8ec67bf75fc3f22d9bfe6cd311dc2719c8d197607a97ea9fa82eb068307', 'alumni'),
(4, 'Guest Viewer', 'viewer@alumniverse.edu', 's3cr3tsalt$e654e8ec67bf75fc3f22d9bfe6cd311dc2719c8d197607a97ea9fa82eb068307', 'viewer');

-- 2. INSTITUTIONS
INSERT INTO institutions (id, name, website) VALUES
(1, 'International Institute of Professional Studies (IIPS DAVV)', 'https://iips.edu.in');

-- 3. DEPARTMENTS
INSERT INTO departments (id, institution_id, name) VALUES
(1, 1, 'Department of Computer Applications'),
(2, 1, 'Department of Management Studies'),
(3, 1, 'Department of Commerce & Finance');

-- 4. BATCHES
INSERT INTO batches (id, department_id, degree, start_year, graduation_year) VALUES
(1, 1, 'MCA (Integrated 5 Year)', 2015, 2020),
(2, 1, 'MCA (Integrated 5 Year)', 2017, 2022),
(3, 1, 'M.Tech (Information Technology)', 2016, 2021),
(4, 1, 'BCA', 2019, 2022),
(5, 2, 'MBA (Management Science)', 2016, 2021),
(6, 2, 'MBA (Tourism)', 2018, 2020),
(7, 3, 'B.Com (Honors)', 2017, 2020);

-- 5. COMPANIES
INSERT INTO companies (id, name, website, industry) VALUES
(1, 'Google', 'https://about.google', 'Technology & Internet'),
(2, 'Microsoft', 'https://microsoft.com', 'Enterprise Software & Cloud'),
(3, 'Amazon AWS', 'https://aws.amazon.com', 'Cloud Computing & E-Commerce'),
(4, 'Adobe Systems', 'https://adobe.com', 'Creative Software & Cloud'),
(5, 'Atlassian', 'https://atlassian.com', 'Collaboration & Dev Tools'),
(6, 'Razorpay', 'https://razorpay.com', 'FinTech & Payments'),
(7, 'CRED', 'https://cred.club', 'FinTech & Consumer Tech'),
(8, 'Tata Consultancy Services', 'https://tcs.com', 'IT Consulting & Services'),
(9, 'Infosys Technologies', 'https://infosys.com', 'IT Consulting & Services'),
(10, 'Zomato', 'https://zomato.com', 'Consumer Tech & Logistics'),
(11, 'Swiggy', 'https://swiggy.com', 'Food Tech & Quick Commerce'),
(12, 'Goldman Sachs', 'https://goldmansachs.com', 'Investment Banking & Finance');

-- 6. SKILLS
INSERT INTO skills (id, name) VALUES
(1, 'C++'),
(2, 'Distributed Systems'),
(3, 'React & JavaScript'),
(4, 'Node.js'),
(5, 'PostgreSQL'),
(6, 'Cloud Architecture (AWS/GCP)'),
(7, 'Product Management'),
(8, 'System Design'),
(9, 'Machine Learning & Python'),
(10, 'Kubernetes & Docker'),
(11, 'Financial Modeling'),
(12, 'Brand Strategy');

-- 7. ALUMNI
INSERT INTO alumni (id, user_id, institution_id, department_id, batch_id, full_name, graduation_year, current_location, bio, linkedin_url, website_url, avatar_url, verification_status, last_verified_at) VALUES
(1, 2, 1, 1, 1, 'Aarav Sharma', 2020, 'Bengaluru, India', 'Staff Software Engineer at Google Core Infrastructure. Passionate about low-latency systems and distributed engines.', 'https://linkedin.com/in/demo-aarav-sharma', 'https://aaravsharma.dev', 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?w=150', 'verified', '2026-01-15 10:00:00+00'),
(2, 3, 1, 1, 1, 'Priya Patel', 2020, 'Seattle, WA, USA', 'Senior Software Engineer at Microsoft Azure Storage. Distributed consensus and cloud resilience specialist.', 'https://linkedin.com/in/demo-priya-patel', 'https://priyapatel.tech', 'https://images.unsplash.com/photo-1494790108377-be9c29b29330?w=150', 'verified', '2026-02-10 14:30:00+00'),
(3, NULL, 1, 1, 1, 'Rohan Verma', 2020, 'Bengaluru, India', 'Principal Architect at Razorpay Payments Gateway. Scaling transactional pipelines to 10k TPS.', 'https://linkedin.com/in/demo-rohan-verma', 'https://rohanverma.io', 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?w=150', 'verified', '2026-01-20 09:15:00+00'),
(4, NULL, 1, 1, 2, 'Ananya Sen', 2022, 'Hyderabad, India', 'Software Development Engineer II at Amazon AWS DynamoDB team. Graduate of MCA 2022.', 'https://linkedin.com/in/demo-ananya-sen', NULL, 'https://images.unsplash.com/photo-1517841905240-472988babdf9?w=150', 'verified', '2026-03-01 11:20:00+00'),
(5, NULL, 1, 1, 2, 'Vikram Malhotra', 2022, 'Bengaluru, India', 'Lead Backend Engineer at CRED. Crafting high-throughput microservices in Go and C++.', 'https://linkedin.com/in/demo-vikram-malhotra', 'https://vikram.codes', 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e?w=150', 'verified', '2026-02-18 16:45:00+00'),
(6, NULL, 1, 1, 3, 'Neha Joshi', 2021, 'Gurugram, India', 'Senior Engineering Manager at Zomato. Overseeing platform reliability and checkout microservices.', 'https://linkedin.com/in/demo-neha-joshi', NULL, 'https://images.unsplash.com/photo-1544005313-94ddf0286df2?w=150', 'verified', '2026-01-12 12:00:00+00'),
(7, NULL, 1, 1, 3, 'Kavya Deshmukh', 2021, 'Pune, India', 'Senior Computer Scientist at Adobe Creative Cloud. Graphics algorithms and WASM integration.', 'https://linkedin.com/in/demo-kavya-deshmukh', 'https://kavyad.art', 'https://images.unsplash.com/photo-1524504388940-b1c1722653e1?w=150', 'verified', '2026-02-28 15:10:00+00'),
(8, NULL, 1, 1, 4, 'Aditya Choudhury', 2022, 'Bengaluru, India', 'Frontend Engineer at Atlassian Jira Cloud. Web performance enthusiast.', 'https://linkedin.com/in/demo-aditya-choudhury', NULL, 'https://images.unsplash.com/photo-1492562080023-ab3db95bfbce?w=150', 'verified', '2026-03-05 10:30:00+00'),
(9, NULL, 1, 2, 5, 'Sneha Kapoor', 2021, 'Mumbai, India', 'Vice President, Product Strategy at Goldman Sachs. Digital wealth platforms and institutional assets.', 'https://linkedin.com/in/demo-sneha-kapoor', NULL, 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?w=150', 'verified', '2026-02-05 13:00:00+00'),
(10, NULL, 1, 2, 5, 'Arjun Nambiar', 2021, 'Bengaluru, India', 'Principal Product Manager at Swiggy Instamart. Supply chain optimization and catalog intelligence.', 'https://linkedin.com/in/demo-arjun-nambiar', 'https://arjunnambiar.me', 'https://images.unsplash.com/photo-1506794778202-cad84cf45f1d?w=150', 'verified', '2026-01-30 08:45:00+00'),
(11, NULL, 1, 2, 6, 'Divya Nair', 2020, 'New Delhi, India', 'Brand Marketing Director at Taj Hospitality Ventures. Experiential tourism campaigns.', 'https://linkedin.com/in/demo-divya-nair', NULL, 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?w=150', 'pending', NULL),
(12, NULL, 1, 3, 7, 'Manish Agarwal', 2020, 'London, United Kingdom', 'Senior Quantitative Analyst at Barclays Investment Bank. Equity derivatives and risk management.', 'https://linkedin.com/in/demo-manish-agarwal', NULL, 'https://images.unsplash.com/photo-1472099645785-5658abf4ff4e?w=150', 'verified', '2026-02-14 17:20:00+00');

-- 8. CAREER HISTORY
INSERT INTO career_history (id, alumni_id, company_id, job_title, start_date, end_date, is_current) VALUES
-- Aarav Sharma: TCS -> Google
(1, 1, 8, 'Systems Engineer', '2020-07-01', '2022-03-15', FALSE),
(2, 1, 1, 'Staff Software Engineer', '2022-04-01', NULL, TRUE),

-- Priya Patel: Infosys -> Microsoft
(3, 2, 9, 'Associate Software Specialist', '2020-07-15', '2022-06-30', FALSE),
(4, 2, 2, 'Senior Software Engineer', '2022-07-15', NULL, TRUE),

-- Rohan Verma: Amazon -> Razorpay
(5, 3, 3, 'Software Development Engineer I', '2020-08-01', '2022-10-31', FALSE),
(6, 3, 6, 'Principal Architect', '2022-11-01', NULL, TRUE),

-- Ananya Sen: Amazon
(7, 4, 3, 'Software Development Engineer II', '2022-07-01', NULL, TRUE),

-- Vikram Malhotra: Razorpay -> CRED
(8, 5, 6, 'Backend Engineer', '2022-07-01', '2024-05-31', FALSE),
(9, 5, 7, 'Lead Backend Engineer', '2024-06-01', NULL, TRUE),

-- Neha Joshi: TCS -> Zomato
(10, 6, 8, 'Senior Systems Analyst', '2021-06-01', '2023-08-31', FALSE),
(11, 6, 10, 'Senior Engineering Manager', '2023-09-01', NULL, TRUE),

-- Kavya Deshmukh: Adobe
(12, 7, 4, 'Senior Computer Scientist', '2021-07-01', NULL, TRUE),

-- Aditya Choudhury: Atlassian
(13, 8, 5, 'Frontend Engineer', '2022-08-01', NULL, TRUE),

-- Sneha Kapoor: Goldman Sachs
(14, 9, 12, 'Vice President, Product Strategy', '2021-06-15', NULL, TRUE),

-- Arjun Nambiar: Zomato -> Swiggy
(15, 10, 10, 'Product Lead', '2021-07-01', '2023-12-31', FALSE),
(16, 10, 11, 'Principal Product Manager', '2024-01-01', NULL, TRUE),

-- Manish Agarwal: Goldman Sachs
(17, 12, 12, 'Senior Quantitative Analyst', '2020-09-01', NULL, TRUE);

-- 9. ALUMNI SKILLS
INSERT INTO alumni_skills (alumni_id, skill_id) VALUES
-- Aarav Sharma: C++, Distributed Systems, System Design, Cloud
(1, 1), (1, 2), (1, 8), (1, 6),
-- Priya Patel: Distributed Systems, Cloud Architecture, C++, Kubernetes
(2, 2), (2, 6), (2, 1), (2, 10),
-- Rohan Verma: System Design, PostgreSQL, Node.js, Distributed Systems
(3, 8), (3, 5), (3, 4), (3, 2),
-- Ananya Sen: Cloud Architecture, Distributed Systems, System Design
(4, 6), (4, 2), (4, 8),
-- Vikram Malhotra: C++, PostgreSQL, Kubernetes, System Design
(5, 1), (5, 5), (5, 10), (5, 8),
-- Neha Joshi: System Design, Product Management, Kubernetes
(6, 8), (6, 7), (6, 10),
-- Kavya Deshmukh: C++, React & JavaScript, System Design
(7, 1), (7, 3), (7, 8),
-- Aditya Choudhury: React & JavaScript, Node.js
(8, 3), (8, 4),
-- Sneha Kapoor: Product Management, Financial Modeling, System Design
(9, 7), (9, 11), (9, 8),
-- Arjun Nambiar: Product Management, System Design
(10, 7), (10, 8),
-- Manish Agarwal: Financial Modeling, Machine Learning & Python
(12, 11), (12, 9);

-- 10. ALUMNI CONNECTIONS (Confirmed professional relationships)
-- Aarav (1) <-> Priya (2) [Classmates 2020 & Colleagues]
-- Aarav (1) <-> Rohan (3) [Classmates 2020]
-- Aarav (1) <-> Ananya (4) [Mentor]
-- Priya (2) <-> Ananya (4) [Professional Connection]
-- Rohan (3) <-> Vikram (5) [Colleagues at Razorpay]
-- Vikram (5) <-> Aditya (8) [Classmates 2022]
-- Neha (6) <-> Arjun (10) [Colleagues at Zomato]
-- Sneha (9) <-> Manish (12) [Colleagues at Goldman Sachs]
-- Priya (2) <-> Kavya (7) [Professional Connection]
INSERT INTO alumni_connections (id, source_alumni_id, target_alumni_id, relationship_type, created_by) VALUES
(1, 1, 2, 'classmate', 1),
(2, 2, 1, 'classmate', 1),
(3, 1, 3, 'classmate', 1),
(4, 3, 1, 'classmate', 1),
(5, 1, 4, 'mentor', 1),
(6, 4, 1, 'mentor', 1),
(7, 2, 4, 'professional_connection', 1),
(8, 4, 2, 'professional_connection', 1),
(9, 3, 5, 'colleague', 1),
(10, 5, 3, 'colleague', 1),
(11, 5, 8, 'classmate', 1),
(12, 8, 5, 'classmate', 1),
(13, 6, 10, 'colleague', 1),
(14, 10, 6, 'colleague', 1),
(15, 9, 12, 'colleague', 1),
(16, 12, 9, 'colleague', 1),
(17, 2, 7, 'professional_connection', 1),
(18, 7, 2, 'professional_connection', 1);

-- 11. AUDIT LOGS
INSERT INTO audit_logs (id, actor_user_id, action, entity_type, entity_id, details) VALUES
(1, 1, 'VERIFY_PROFILE', 'alumni', 1, 'Admin verified Aarav Sharma profile credentials via official Google work email.'),
(2, 1, 'VERIFY_PROFILE', 'alumni', 2, 'Admin verified Priya Patel profile credentials via Microsoft work email.'),
(3, 1, 'SEED_DATABASE', 'system', 0, 'Initial database migration and realistic fictional seed completed.');

-- Set sequence values to continue past seeded IDs
SELECT setval(pg_get_serial_sequence('users', 'id'), coalesce(max(id), 1)) FROM users;
SELECT setval(pg_get_serial_sequence('institutions', 'id'), coalesce(max(id), 1)) FROM institutions;
SELECT setval(pg_get_serial_sequence('departments', 'id'), coalesce(max(id), 1)) FROM departments;
SELECT setval(pg_get_serial_sequence('batches', 'id'), coalesce(max(id), 1)) FROM batches;
SELECT setval(pg_get_serial_sequence('alumni', 'id'), coalesce(max(id), 1)) FROM alumni;
SELECT setval(pg_get_serial_sequence('companies', 'id'), coalesce(max(id), 1)) FROM companies;
SELECT setval(pg_get_serial_sequence('career_history', 'id'), coalesce(max(id), 1)) FROM career_history;
SELECT setval(pg_get_serial_sequence('skills', 'id'), coalesce(max(id), 1)) FROM skills;
SELECT setval(pg_get_serial_sequence('alumni_connections', 'id'), coalesce(max(id), 1)) FROM alumni_connections;
SELECT setval(pg_get_serial_sequence('audit_logs', 'id'), coalesce(max(id), 1)) FROM audit_logs;
