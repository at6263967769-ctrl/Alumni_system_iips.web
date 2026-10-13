-- AlumniVerse Database Schema Migration
-- Migration: 001_initial_schema.sql

-- Enable UUID extension if needed (optional)
-- CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- 1. USERS TABLE
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    role VARCHAR(50) NOT NULL CHECK (role IN ('admin', 'alumni', 'viewer')),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_users_email ON users(email);
CREATE INDEX IF NOT EXISTS idx_users_role ON users(role);

-- 2. INSTITUTIONS TABLE
CREATE TABLE IF NOT EXISTS institutions (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE,
    website VARCHAR(255),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- 3. DEPARTMENTS TABLE
CREATE TABLE IF NOT EXISTS departments (
    id SERIAL PRIMARY KEY,
    institution_id INTEGER NOT NULL REFERENCES institutions(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    CONSTRAINT uq_institution_dept UNIQUE (institution_id, name)
);

CREATE INDEX IF NOT EXISTS idx_departments_institution ON departments(institution_id);

-- 4. BATCHES TABLE
CREATE TABLE IF NOT EXISTS batches (
    id SERIAL PRIMARY KEY,
    department_id INTEGER NOT NULL REFERENCES departments(id) ON DELETE CASCADE,
    degree VARCHAR(100) NOT NULL,
    start_year INTEGER NOT NULL,
    graduation_year INTEGER NOT NULL,
    CONSTRAINT uq_batch_dept_degree_year UNIQUE (department_id, degree, graduation_year)
);

CREATE INDEX IF NOT EXISTS idx_batches_department ON batches(department_id);
CREATE INDEX IF NOT EXISTS idx_batches_grad_year ON batches(graduation_year);

-- 5. ALUMNI TABLE
CREATE TABLE IF NOT EXISTS alumni (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id) ON DELETE SET NULL,
    institution_id INTEGER NOT NULL REFERENCES institutions(id) ON DELETE RESTRICT,
    department_id INTEGER NOT NULL REFERENCES departments(id) ON DELETE RESTRICT,
    batch_id INTEGER NOT NULL REFERENCES batches(id) ON DELETE RESTRICT,
    full_name VARCHAR(255) NOT NULL,
    graduation_year INTEGER NOT NULL,
    current_location VARCHAR(255),
    bio TEXT,
    linkedin_url VARCHAR(500),
    website_url VARCHAR(500),
    avatar_url VARCHAR(500),
    verification_status VARCHAR(50) NOT NULL DEFAULT 'pending' CHECK (verification_status IN ('pending', 'verified', 'rejected')),
    last_verified_at TIMESTAMP WITH TIME ZONE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_alumni_institution ON alumni(institution_id);
CREATE INDEX IF NOT EXISTS idx_alumni_department ON alumni(department_id);
CREATE INDEX IF NOT EXISTS idx_alumni_batch ON alumni(batch_id);
CREATE INDEX IF NOT EXISTS idx_alumni_grad_year ON alumni(graduation_year);
CREATE INDEX IF NOT EXISTS idx_alumni_verification ON alumni(verification_status);
CREATE INDEX IF NOT EXISTS idx_alumni_full_name ON alumni(full_name);

-- 6. COMPANIES TABLE
CREATE TABLE IF NOT EXISTS companies (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE,
    website VARCHAR(255),
    industry VARCHAR(100) NOT NULL
);

CREATE INDEX IF NOT EXISTS idx_companies_industry ON companies(industry);
CREATE INDEX IF NOT EXISTS idx_companies_name ON companies(name);

-- 7. CAREER HISTORY TABLE
CREATE TABLE IF NOT EXISTS career_history (
    id SERIAL PRIMARY KEY,
    alumni_id INTEGER NOT NULL REFERENCES alumni(id) ON DELETE CASCADE,
    company_id INTEGER NOT NULL REFERENCES companies(id) ON DELETE RESTRICT,
    job_title VARCHAR(255) NOT NULL,
    start_date DATE NOT NULL,
    end_date DATE,
    is_current BOOLEAN NOT NULL DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_career_alumni ON career_history(alumni_id);
CREATE INDEX IF NOT EXISTS idx_career_company ON career_history(company_id);
CREATE INDEX IF NOT EXISTS idx_career_is_current ON career_history(is_current);

-- Enforce at most one current job per alumni using a partial unique index
CREATE UNIQUE INDEX IF NOT EXISTS uq_alumni_single_current_job 
ON career_history(alumni_id) 
WHERE is_current = TRUE;

-- 8. SKILLS TABLE
CREATE TABLE IF NOT EXISTS skills (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE
);

CREATE INDEX IF NOT EXISTS idx_skills_name ON skills(name);

-- 9. ALUMNI SKILLS JUNCTION
CREATE TABLE IF NOT EXISTS alumni_skills (
    alumni_id INTEGER NOT NULL REFERENCES alumni(id) ON DELETE CASCADE,
    skill_id INTEGER NOT NULL REFERENCES skills(id) ON DELETE CASCADE,
    PRIMARY KEY (alumni_id, skill_id)
);

CREATE INDEX IF NOT EXISTS idx_alumni_skills_skill ON alumni_skills(skill_id);

-- 10. ALUMNI CONNECTIONS TABLE
CREATE TABLE IF NOT EXISTS alumni_connections (
    id SERIAL PRIMARY KEY,
    source_alumni_id INTEGER NOT NULL REFERENCES alumni(id) ON DELETE CASCADE,
    target_alumni_id INTEGER NOT NULL REFERENCES alumni(id) ON DELETE CASCADE,
    relationship_type VARCHAR(50) NOT NULL CHECK (relationship_type IN ('colleague', 'mentor', 'classmate', 'professional_connection')),
    created_by INTEGER REFERENCES users(id) ON DELETE SET NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT chk_no_self_connection CHECK (source_alumni_id <> target_alumni_id),
    CONSTRAINT uq_alumni_connection_pair UNIQUE (source_alumni_id, target_alumni_id, relationship_type)
);

CREATE INDEX IF NOT EXISTS idx_connections_source ON alumni_connections(source_alumni_id);
CREATE INDEX IF NOT EXISTS idx_connections_target ON alumni_connections(target_alumni_id);
CREATE INDEX IF NOT EXISTS idx_connections_type ON alumni_connections(relationship_type);

-- 11. AUDIT LOGS TABLE
CREATE TABLE IF NOT EXISTS audit_logs (
    id SERIAL PRIMARY KEY,
    actor_user_id INTEGER REFERENCES users(id) ON DELETE SET NULL,
    action VARCHAR(100) NOT NULL,
    entity_type VARCHAR(100) NOT NULL,
    entity_id INTEGER,
    details TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_audit_logs_actor ON audit_logs(actor_user_id);
CREATE INDEX IF NOT EXISTS idx_audit_logs_entity ON audit_logs(entity_type, entity_id);
CREATE INDEX IF NOT EXISTS idx_audit_logs_created_at ON audit_logs(created_at);
