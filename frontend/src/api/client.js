import axios from 'axios';

const API_BASE_URL = import.meta.env.VITE_API_URL || '/api/v1';

const api = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'text/plain',
  },
  transformRequest: [(data) => {
    if (data && typeof data === 'object') {
      return JSON.stringify(data);
    }
    return data;
  }],
});

// Attach Authorization Bearer token only when needed to prevent CORS preflight on read/auth routes
api.interceptors.request.use((config) => {
  const token = localStorage.getItem('alumniverse_token');
  const isPublicRead = config.method === 'get' && !config.url?.includes('/auth/me') && !config.url?.includes('/admin/');
  const isLoginOrRegister = config.url?.includes('/auth/login') || config.url?.includes('/auth/register');

  if (token && !isPublicRead && !isLoginOrRegister) {
    config.headers.Authorization = `Bearer ${token}`;
  }
  return config;
}, (error) => Promise.reject(error));

// Extract response data uniformly
api.interceptors.response.use(
  (response) => response.data,
  (error) => {
    let errorMsg = error.response?.data?.message || error.message || 'An unexpected API error occurred.';
    if (error.message === 'Network Error' || !error.response) {
      errorMsg = `Network Error: Cannot reach Backend API at [${API_BASE_URL}]. Please verify backend service URL in Render Environment settings.`;
    }
    return Promise.reject(new Error(errorMsg));
  }
);

export const apiClient = {
  // Health
  getHealth: () => api.get('/health'),

  // Auth
  login: (email, password) => api.post('/auth/login', { email, password }),
  register: (name, email, password, role = 'alumni') => api.post('/auth/register', { name, email, password, role }),
  logout: () => api.post('/auth/logout'),
  getMe: () => api.get('/auth/me'),

  // Tree
  getTree: (hierarchy = 'academic', params = {}) => api.get('/tree', { params: { hierarchy, ...params } }),
  getSubtree: (nodeId, hierarchy = 'academic', params = {}) => api.get(`/tree/${encodeURIComponent(nodeId)}/subtree`, { params: { hierarchy, ...params } }),
  getNodeDetails: (nodeId, hierarchy = 'academic') => api.get(`/tree/${encodeURIComponent(nodeId)}`, { params: { hierarchy } }),

  // Graph
  getGraph: (params = {}) => api.get('/graph', { params }),
  getNeighbors: (id) => api.get(`/graph/${id}/neighbors`),
  getConnections: (id) => api.get(`/graph/${id}/connections`),
  findShortestPath: (source, target) => api.get('/graph/path', { params: { source, target } }),

  // Alumni CRUD & Directory
  getAlumniList: (params = {}) => api.get('/alumni', { params }),
  getAlumniById: (id) => api.get(`/alumni/${id}`),
  createAlumni: (data) => api.post('/alumni', data),
  updateAlumni: (id, data) => api.put(`/alumni/${id}`, data),
  deleteAlumni: (id) => api.delete(`/alumni/${id}`),

  // Companies & Metadata
  getCompanies: () => api.get('/companies'),
  getCompanyAlumni: (id) => api.get(`/companies/${id}/alumni`),
  getMetadata: () => api.get('/metadata'),

  // Admin & Analytics
  getAnalytics: () => api.get('/admin/analytics'),
  verifyAlumni: (id, status = 'verified') => api.post(`/admin/alumni/${id}/verify`, { status }),
  importCsv: (csvContent) => api.post('/admin/import/csv', csvContent, {
    headers: { 'Content-Type': 'text/plain' }
  }),
  addConnection: (sourceId, targetId, relationshipType) => api.post('/admin/connections', {
    source_alumni_id: sourceId,
    target_alumni_id: targetId,
    relationship_type: relationshipType
  }),
  getAuditLogs: () => api.get('/admin/audit-logs'),
};

export default apiClient;
