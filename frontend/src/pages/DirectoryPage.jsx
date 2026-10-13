import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import {
  Search,
  Filter,
  Grid,
  List,
  Building2,
  GraduationCap,
  ShieldCheck,
  Clock,
  ArrowRight,
  ExternalLink,
  ChevronLeft,
  ChevronRight,
  Sparkles,
} from 'lucide-react';
import apiClient from '../api/client';

export const DirectoryPage = () => {
  const [alumniList, setAlumniList] = useState([]);
  const [pagination, setPagination] = useState({ page: 1, limit: 9, total: 0, pages: 1 });
  const [loading, setLoading] = useState(true);
  const [viewStyle, setViewStyle] = useState('grid'); // 'grid' | 'table'

  const [search, setSearch] = useState('');
  const [selectedBatch, setSelectedBatch] = useState('');
  const [selectedCompany, setSelectedCompany] = useState('');
  const [selectedStatus, setSelectedStatus] = useState('');
  const [sortBy, setSortBy] = useState('full_name');
  const [sortOrder, setSortOrder] = useState('asc');

  const [metadata, setMetadata] = useState(null);

  useEffect(() => {
    const fetchMeta = async () => {
      try {
        const res = await apiClient.getMetadata();
        if (res.status === 'success') {
          setMetadata(res.data);
        }
      } catch (err) {
        console.warn('Metadata load error:', err.message);
      }
    };
    fetchMeta();
  }, []);

  const fetchDirectory = async (page = 1) => {
    setLoading(true);
    try {
      const params = {
        page,
        limit: pagination.limit,
        sort_by: sortBy,
        sort_order: sortOrder,
      };
      if (search) params.search = search;
      if (selectedBatch) params.batch_id = selectedBatch;
      if (selectedCompany) params.company = selectedCompany;
      if (selectedStatus) params.verification_status = selectedStatus;

      const res = await apiClient.getAlumniList(params);
      if (res.status === 'success') {
        setAlumniList(res.data.items || []);
        setPagination(res.data.pagination);
      }
    } catch (err) {
      console.error('Directory fetch error:', err.message);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchDirectory(1);
  }, [search, selectedBatch, selectedCompany, selectedStatus, sortBy, sortOrder]);

  return (
    <div className="min-h-screen bg-[#080B12] text-slate-100 p-4 sm:p-6 lg:p-8 max-w-7xl mx-auto space-y-6">
      {/* Page Title */}
      <div className="flex flex-col sm:flex-row sm:items-center justify-between gap-4 pb-4 border-b border-slate-800">
        <div>
          <h1 className="text-2xl font-black text-white tracking-tight flex items-center gap-2.5">
            <GraduationCap className="w-6 h-6 text-purple-400" />
            Alumni Directory & Records
          </h1>
          <p className="text-slate-400 text-xs mt-1">
            Searchable index of verified alumni across graduating batches, degrees, and companies.
          </p>
        </div>

        <div className="flex items-center gap-2 self-start sm:self-auto">
          <div className="p-1 rounded-xl bg-slate-900 border border-slate-800 flex items-center">
            <button
              onClick={() => setViewStyle('grid')}
              className={`p-1.5 rounded-lg text-xs font-semibold ${
                viewStyle === 'grid' ? 'bg-cyan-500 text-slate-950' : 'text-slate-400 hover:text-white'
              }`}
              title="Grid Cards"
            >
              <Grid className="w-4 h-4" />
            </button>
            <button
              onClick={() => setViewStyle('table')}
              className={`p-1.5 rounded-lg text-xs font-semibold ${
                viewStyle === 'table' ? 'bg-cyan-500 text-slate-950' : 'text-slate-400 hover:text-white'
              }`}
              title="Table View"
            >
              <List className="w-4 h-4" />
            </button>
          </div>
        </div>
      </div>

      {/* Filter and Search Bar */}
      <div className="p-4 rounded-2xl glass-panel border border-slate-800 grid grid-cols-1 sm:grid-cols-2 md:grid-cols-4 gap-3 text-xs">
        {/* Search Input */}
        <div className="relative">
          <Search className="w-4 h-4 absolute left-3 top-2.5 text-slate-500" />
          <input
            type="text"
            placeholder="Search by name, role, skill..."
            value={search}
            onChange={(e) => setSearch(e.target.value)}
            className="w-full pl-9 pr-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white placeholder-slate-500 focus:outline-none focus:border-cyan-400"
          />
        </div>

        {/* Batch Filter */}
        <select
          value={selectedBatch}
          onChange={(e) => setSelectedBatch(e.target.value)}
          className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white focus:outline-none focus:border-cyan-400"
        >
          <option value="">All Batches</option>
          {metadata?.batches?.map((b) => (
            <option key={b.id} value={b.id}>
              {b.degree} ({b.graduation_year})
            </option>
          ))}
        </select>

        {/* Company Filter */}
        <select
          value={selectedCompany}
          onChange={(e) => setSelectedCompany(e.target.value)}
          className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white focus:outline-none focus:border-cyan-400"
        >
          <option value="">All Companies</option>
          {metadata?.companies?.map((c) => (
            <option key={c.id} value={c.name}>
              {c.name}
            </option>
          ))}
        </select>

        {/* Sort selector */}
        <div className="flex gap-2">
          <select
            value={sortBy}
            onChange={(e) => setSortBy(e.target.value)}
            className="flex-1 px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white focus:outline-none focus:border-cyan-400"
          >
            <option value="full_name">Sort: Name</option>
            <option value="graduation_year">Sort: Year</option>
            <option value="updated_at">Sort: Updated</option>
          </select>
          <button
            onClick={() => setSortOrder(sortOrder === 'asc' ? 'desc' : 'asc')}
            className="px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-slate-300 font-bold hover:text-cyan-400"
            title="Toggle sort direction"
          >
            {sortOrder.toUpperCase()}
          </button>
        </div>
      </div>

      {/* Directory Content */}
      {loading ? (
        <div className="py-20 text-center flex flex-col items-center justify-center gap-2 text-cyan-400">
          <Sparkles className="w-6 h-6 animate-spin" />
          <span className="text-xs font-mono">Fetching alumni records...</span>
        </div>
      ) : alumniList.length === 0 ? (
        <div className="py-16 text-center text-slate-400 p-8 rounded-2xl glass-panel border border-slate-800">
          <p className="font-bold text-sm text-slate-300 mb-1">No alumni found</p>
          <p className="text-xs">Try adjusting your search keywords or clearing active filters.</p>
        </div>
      ) : viewStyle === 'grid' ? (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-5">
          {alumniList.map((al) => {
            const isVerified = al.verification_status === 'verified';
            return (
              <div
                key={al.id}
                className="p-5 rounded-2xl glass-panel border border-slate-800 hover:border-cyan-500/50 transition-all flex flex-col justify-between group shadow-lg"
              >
                <div>
                  <div className="flex items-start justify-between gap-3 mb-3">
                    <div className="flex items-center gap-3">
                      <div className="w-12 h-12 rounded-xl bg-gradient-to-tr from-cyan-600 to-indigo-600 text-white font-extrabold text-sm flex items-center justify-center shrink-0 border border-cyan-400/40">
                        {al.full_name?.slice(0, 2).toUpperCase()}
                      </div>
                      <div>
                        <h3 className="font-bold text-sm text-white group-hover:text-cyan-300 transition-colors">
                          {al.full_name}
                        </h3>
                        <p className="text-cyan-400 font-semibold text-xs flex items-center gap-1">
                          <Building2 className="w-3.5 h-3.5" />
                          {al.current_company}
                        </p>
                        <p className="text-slate-400 text-[11px]">{al.current_job_title}</p>
                      </div>
                    </div>

                    {isVerified ? (
                      <span className="text-emerald-400 shrink-0" title="Verified Profile">
                        <ShieldCheck className="w-4 h-4" />
                      </span>
                    ) : (
                      <span className="text-amber-400 shrink-0" title="Pending Verification">
                        <Clock className="w-4 h-4" />
                      </span>
                    )}
                  </div>

                  <div className="p-2.5 rounded-xl bg-slate-900/60 border border-slate-800/80 mb-3 text-xs space-y-1">
                    <div className="flex justify-between text-slate-400">
                      <span>Degree:</span>
                      <span className="text-slate-200 font-medium">{al.degree}</span>
                    </div>
                    <div className="flex justify-between text-slate-400">
                      <span>Class of:</span>
                      <span className="text-purple-300 font-semibold">{al.graduation_year}</span>
                    </div>
                  </div>

                  {al.skills?.length > 0 && (
                    <div className="flex flex-wrap gap-1 mb-4">
                      {al.skills.slice(0, 3).map((sk, idx) => (
                        <span
                          key={idx}
                          className="px-2 py-0.5 rounded bg-slate-800/80 text-cyan-300 border border-slate-700/60 font-mono text-[9px]"
                        >
                          {sk}
                        </span>
                      ))}
                    </div>
                  )}
                </div>

                <div className="pt-3 border-t border-slate-800/80 flex items-center justify-between">
                  <Link
                    to={`/alumni/${al.id}`}
                    className="text-xs font-semibold text-cyan-400 hover:text-cyan-300 flex items-center gap-1"
                  >
                    View Dossier <ArrowRight className="w-3.5 h-3.5" />
                  </Link>

                  {al.linkedin_url && (
                    <a
                      href={al.linkedin_url}
                      target="_blank"
                      rel="noopener noreferrer"
                      className="text-slate-400 hover:text-white"
                      title="LinkedIn Profile"
                    >
                      <ExternalLink className="w-3.5 h-3.5" />
                    </a>
                  )}
                </div>
              </div>
            );
          })}
        </div>
      ) : (
        /* Table View */
        <div className="overflow-x-auto rounded-2xl glass-panel border border-slate-800">
          <table className="w-full text-left text-xs">
            <thead className="bg-slate-900/80 border-b border-slate-800 text-slate-400 uppercase font-semibold">
              <tr>
                <th className="p-3.5">Alumni Name</th>
                <th className="p-3.5">Company & Role</th>
                <th className="p-3.5">Degree & Year</th>
                <th className="p-3.5">Location</th>
                <th className="p-3.5">Verification</th>
                <th className="p-3.5 text-right">Actions</th>
              </tr>
            </thead>
            <tbody className="divide-y divide-slate-800/60">
              {alumniList.map((al) => (
                <tr key={al.id} className="hover:bg-slate-800/30 transition-colors">
                  <td className="p-3.5 font-bold text-white">{al.full_name}</td>
                  <td className="p-3.5">
                    <span className="text-cyan-400 font-medium block">{al.current_company}</span>
                    <span className="text-slate-400 text-[11px]">{al.current_job_title}</span>
                  </td>
                  <td className="p-3.5 text-slate-300">
                    {al.degree} ({al.graduation_year})
                  </td>
                  <td className="p-3.5 text-slate-400">{al.current_location || '-'}</td>
                  <td className="p-3.5">
                    <span
                      className={`inline-flex items-center gap-1 px-2 py-0.5 rounded-full text-[10px] font-semibold border ${
                        al.verification_status === 'verified'
                          ? 'bg-emerald-950 text-emerald-300 border-emerald-800/50'
                          : 'bg-amber-950 text-amber-300 border-amber-800/50'
                      }`}
                    >
                      {al.verification_status}
                    </span>
                  </td>
                  <td className="p-3.5 text-right">
                    <Link
                      to={`/alumni/${al.id}`}
                      className="text-cyan-400 hover:text-cyan-300 font-semibold"
                    >
                      Inspect
                    </Link>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      )}

      {/* Pagination Controls */}
      <div className="flex items-center justify-between pt-4 border-t border-slate-800 text-xs text-slate-400">
        <span>
          Showing page <strong>{pagination.page}</strong> of <strong>{pagination.pages || 1}</strong> ({pagination.total} total alumni)
        </span>
        <div className="flex items-center gap-2">
          <button
            disabled={pagination.page <= 1}
            onClick={() => fetchDirectory(pagination.page - 1)}
            className="p-2 rounded-lg bg-slate-900 border border-slate-800 text-white disabled:opacity-40 disabled:cursor-not-allowed hover:bg-slate-800"
          >
            <ChevronLeft className="w-4 h-4" />
          </button>
          <button
            disabled={pagination.page >= pagination.pages}
            onClick={() => fetchDirectory(pagination.page + 1)}
            className="p-2 rounded-lg bg-slate-900 border border-slate-800 text-white disabled:opacity-40 disabled:cursor-not-allowed hover:bg-slate-800"
          >
            <ChevronRight className="w-4 h-4" />
          </button>
        </div>
      </div>
    </div>
  );
};

export default DirectoryPage;
