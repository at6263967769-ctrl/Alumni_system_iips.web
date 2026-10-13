import React, { useState, useEffect } from 'react';
import { useParams, Link } from 'react-router-dom';
import {
  ShieldCheck,
  Clock,
  Building2,
  GraduationCap,
  MapPin,
  ExternalLink,
  Briefcase,
  Users,
  Calendar,
  ArrowLeft,
  Sparkles,
  Network,
} from 'lucide-react';
import apiClient from '../api/client';

export const ProfileDetailPage = () => {
  const { id } = useParams();
  const [alumni, setAlumni] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    const fetchAlumni = async () => {
      setLoading(true);
      try {
        const res = await apiClient.getAlumniById(id);
        if (res.status === 'success') {
          setAlumni(res.data);
        } else {
          setError(res.message || 'Alumni profile not found.');
        }
      } catch (err) {
        setError(err.message || 'Error communicating with backend.');
      } finally {
        setLoading(false);
      }
    };
    fetchAlumni();
  }, [id]);

  if (loading) {
    return (
      <div className="min-h-screen bg-[#080B12] p-8 flex items-center justify-center text-cyan-400">
        <Sparkles className="w-8 h-8 animate-spin" />
      </div>
    );
  }

  if (error || !alumni) {
    return (
      <div className="min-h-screen bg-[#080B12] p-8 flex items-center justify-center text-center">
        <div className="max-w-md p-6 rounded-2xl glass-panel border border-slate-800 text-slate-300">
          <p className="font-bold text-sm text-white mb-2">Profile Not Found</p>
          <p className="text-xs text-slate-400 mb-4">{error || 'Could not locate alumni record.'}</p>
          <Link
            to="/directory"
            className="px-4 py-2 rounded-lg bg-cyan-600 text-slate-950 font-bold text-xs"
          >
            Return to Directory
          </Link>
        </div>
      </div>
    );
  }

  const isVerified = alumni.verification_status === 'verified';
  const name = alumni.full_name || 'Alumni Member';
  const avatar = alumni.avatar_url;
  const initials = name
    .split(' ')
    .map((n) => n[0])
    .join('')
    .slice(0, 2)
    .toUpperCase();

  return (
    <div className="min-h-screen bg-[#080B12] text-slate-100 p-4 sm:p-6 lg:p-8 max-w-5xl mx-auto space-y-6">
      {/* Back Button */}
      <div>
        <Link
          to="/directory"
          className="inline-flex items-center gap-2 text-xs font-semibold text-slate-400 hover:text-cyan-400 transition-colors"
        >
          <ArrowLeft className="w-4 h-4" />
          Back to Directory
        </Link>
      </div>

      {/* Main Header Profile Card */}
      <div className="p-6 sm:p-8 rounded-3xl glass-panel border border-slate-800 relative overflow-hidden shadow-2xl">
        <div className="flex flex-col sm:flex-row items-start sm:items-center justify-between gap-6">
          <div className="flex items-center gap-5">
            <div className="relative shrink-0">
              {avatar ? (
                <img
                  src={avatar}
                  alt={name}
                  className="w-20 h-20 rounded-2xl object-cover border-2 border-cyan-400 shadow-xl shadow-cyan-500/20"
                />
              ) : (
                <div className="w-20 h-20 rounded-2xl bg-gradient-to-tr from-cyan-600 via-indigo-600 to-purple-600 text-white font-extrabold text-2xl flex items-center justify-center border-2 border-cyan-400 shadow-xl shadow-cyan-500/20">
                  {initials}
                </div>
              )}
              {isVerified && (
                <span className="absolute -bottom-1 -right-1 bg-emerald-500 text-slate-950 p-1 rounded-full ring-4 ring-[#0B101D]">
                  <ShieldCheck className="w-4 h-4 text-white" />
                </span>
              )}
            </div>

            <div>
              <div className="flex items-center gap-3">
                <h1 className="text-2xl font-black text-white">{name}</h1>
                <span
                  className={`px-2.5 py-0.5 rounded-full text-[10px] font-bold uppercase tracking-wider border ${
                    isVerified
                      ? 'bg-emerald-950 text-emerald-300 border-emerald-800'
                      : 'bg-amber-950 text-amber-300 border-amber-800'
                  }`}
                >
                  {alumni.verification_status}
                </span>
              </div>
              <p className="text-cyan-400 font-semibold text-sm flex items-center gap-1.5 mt-1">
                <Building2 className="w-4 h-4" />
                {alumni.current_company} &bull; {alumni.current_job_title}
              </p>
              {alumni.current_location && (
                <p className="text-slate-400 text-xs flex items-center gap-1 mt-1">
                  <MapPin className="w-3.5 h-3.5 text-slate-500" />
                  {alumni.current_location}
                </p>
              )}
            </div>
          </div>

          <div className="flex flex-wrap sm:flex-col gap-2 w-full sm:w-auto">
            <Link
              to={`/explorer?search=${encodeURIComponent(alumni.full_name)}`}
              className="px-4 py-2 rounded-xl bg-gradient-to-r from-cyan-500 to-indigo-600 hover:from-cyan-400 text-slate-950 font-bold text-xs flex items-center justify-center gap-2 shadow-lg transition-all"
            >
              <Network className="w-4 h-4" />
              Locate on Tree Canvas
            </Link>

            {alumni.linkedin_url && (
              <a
                href={alumni.linkedin_url}
                target="_blank"
                rel="noopener noreferrer"
                className="px-4 py-2 rounded-xl bg-slate-900 border border-slate-700 hover:border-slate-500 text-white font-semibold text-xs flex items-center justify-center gap-2 transition-all"
              >
                <ExternalLink className="w-4 h-4" />
                View LinkedIn
              </a>
            )}
          </div>
        </div>

        {alumni.bio && (
          <div className="mt-6 pt-6 border-t border-slate-800/80">
            <h3 className="text-xs font-bold uppercase tracking-wider text-slate-400 mb-2">Executive Biography</h3>
            <p className="text-slate-300 text-sm leading-relaxed max-w-3xl">{alumni.bio}</p>
          </div>
        )}
      </div>

      {/* Grid: Credentials & Employment */}
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        {/* Academic Credentials */}
        <div className="p-6 rounded-2xl glass-panel border border-slate-800 space-y-4">
          <h3 className="text-sm font-bold text-white uppercase tracking-wider flex items-center gap-2">
            <GraduationCap className="w-4 h-4 text-purple-400" />
            Institutional Education
          </h3>
          <div className="p-4 rounded-xl bg-slate-900/60 border border-slate-800 space-y-2 text-xs">
            <div className="flex justify-between">
              <span className="text-slate-400">Institution:</span>
              <span className="text-white font-semibold">{alumni.institution_name}</span>
            </div>
            <div className="flex justify-between">
              <span className="text-slate-400">Department:</span>
              <span className="text-white">{alumni.department_name}</span>
            </div>
            <div className="flex justify-between">
              <span className="text-slate-400">Degree:</span>
              <span className="text-cyan-400 font-bold">{alumni.degree}</span>
            </div>
            <div className="flex justify-between">
              <span className="text-slate-400">Graduation Year:</span>
              <span className="text-purple-300 font-mono font-bold">{alumni.graduation_year}</span>
            </div>
          </div>

          {/* Skills */}
          {alumni.skills?.length > 0 && (
            <div>
              <h4 className="text-xs font-bold uppercase tracking-wider text-slate-400 mb-2">Verified Skillsets</h4>
              <div className="flex flex-wrap gap-1.5">
                {alumni.skills.map((sk, idx) => (
                  <span
                    key={idx}
                    className="px-2.5 py-1 rounded-lg bg-slate-800/80 text-cyan-300 border border-slate-700/60 font-mono text-xs"
                  >
                    {sk}
                  </span>
                ))}
              </div>
            </div>
          )}
        </div>

        {/* Career Timeline */}
        <div className="p-6 rounded-2xl glass-panel border border-slate-800 space-y-4">
          <h3 className="text-sm font-bold text-white uppercase tracking-wider flex items-center gap-2">
            <Briefcase className="w-4 h-4 text-cyan-400" />
            Career History
          </h3>
          {alumni.career_history?.length > 0 ? (
            <div className="space-y-3 border-l-2 border-slate-800 ml-2 pl-4 text-xs">
              {alumni.career_history.map((job) => (
                <div key={job.id} className="relative">
                  <div className={`w-3 h-3 rounded-full absolute -left-[22px] top-1.5 ${job.is_current ? 'bg-cyan-400 ring-4 ring-cyan-950' : 'bg-slate-600'}`} />
                  <div className="p-3 rounded-xl bg-slate-900/80 border border-slate-800">
                    <div className="flex items-center justify-between">
                      <span className="font-bold text-white text-xs">{job.job_title}</span>
                      {job.is_current && (
                        <span className="text-[10px] uppercase font-bold px-1.5 py-0.5 rounded bg-cyan-950 text-cyan-300 border border-cyan-800">
                          Current
                        </span>
                      )}
                    </div>
                    <p className="text-cyan-400 font-medium text-xs mt-0.5">{job.company_name} ({job.industry})</p>
                    <p className="text-slate-500 text-[10px] flex items-center gap-1 mt-1">
                      <Calendar className="w-3 h-3" />
                      {job.start_date} &rarr; {job.end_date || 'Present'}
                    </p>
                  </div>
                </div>
              ))}
            </div>
          ) : (
            <p className="text-slate-400 text-xs">No career history entries recorded.</p>
          )}
        </div>
      </div>

      {/* Confirmed Relationships */}
      {alumni.connections?.length > 0 && (
        <div className="p-6 rounded-2xl glass-panel border border-slate-800 space-y-4">
          <h3 className="text-sm font-bold text-white uppercase tracking-wider flex items-center gap-2">
            <Users className="w-4 h-4 text-indigo-400" />
            Confirmed Graph Connections ({alumni.connections.length})
          </h3>
          <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-3">
            {alumni.connections.map((c) => (
              <Link
                key={c.id}
                to={`/alumni/${c.target_alumni_id}`}
                className="p-3 rounded-xl bg-slate-900/60 hover:bg-slate-800/80 border border-slate-800 hover:border-cyan-500/40 transition-all text-xs flex items-center justify-between"
              >
                <div>
                  <h4 className="font-bold text-white">{c.target_name}</h4>
                  <p className="text-[11px] text-cyan-400">{c.target_company}</p>
                  <p className="text-[10px] text-slate-400">{c.target_job_title}</p>
                </div>
                <span className="text-[10px] uppercase px-2 py-0.5 rounded bg-indigo-950 text-indigo-300 border border-indigo-800/40 font-semibold shrink-0">
                  {c.relationship_type}
                </span>
              </Link>
            ))}
          </div>
        </div>
      )}
    </div>
  );
};

export default ProfileDetailPage;
