import React from 'react';
import {
  X,
  ShieldCheck,
  Clock,
  Building2,
  GraduationCap,
  MapPin,
  ExternalLink,
  Briefcase,
  Users,
  Calendar,
  Sparkles,
} from 'lucide-react';

export const AlumniDetailPanel = ({ alumni, onClose, onSelectAlumniById }) => {
  if (!alumni) return null;

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
    <div className="fixed inset-y-0 right-0 w-96 max-w-[90vw] glass-panel bg-[#0B101D]/95 border-l border-slate-800 shadow-2xl z-50 flex flex-col transition-transform duration-300 backdrop-blur-xl">
      {/* Header */}
      <div className="p-4 border-b border-slate-800/80 flex items-center justify-between bg-slate-900/60">
        <div className="flex items-center gap-2">
          <Sparkles className="w-4 h-4 text-cyan-400" />
          <h3 className="text-xs font-bold uppercase tracking-wider text-slate-200">Alumni Intelligence Dossier</h3>
        </div>
        <button
          onClick={onClose}
          className="p-1.5 rounded-lg text-slate-400 hover:text-white hover:bg-slate-800 transition-colors"
          title="Close panel"
        >
          <X className="w-4 h-4" />
        </button>
      </div>

      {/* Body Content */}
      <div className="flex-1 overflow-y-auto p-5 space-y-5 text-xs">
        {/* Profile Card */}
        <div className="flex items-start gap-3.5">
          <div className="relative shrink-0">
            {avatar ? (
              <img
                src={avatar}
                alt={name}
                className="w-14 h-14 rounded-2xl object-cover border-2 border-cyan-400/50 shadow-md"
              />
            ) : (
              <div className="w-14 h-14 rounded-2xl bg-gradient-to-tr from-cyan-600 via-indigo-600 to-purple-600 text-white font-extrabold text-lg flex items-center justify-center border-2 border-cyan-400/50 shadow-md">
                {initials}
              </div>
            )}
            {isVerified && (
              <span className="absolute -bottom-1 -right-1 bg-emerald-500 text-slate-950 p-1 rounded-full ring-2 ring-[#0B101D]">
                <ShieldCheck className="w-3.5 h-3.5 text-white" />
              </span>
            )}
          </div>

          <div className="min-w-0 flex-1">
            <h2 className="text-sm font-extrabold text-white truncate">{name}</h2>
            <p className="text-cyan-400 font-semibold text-xs flex items-center gap-1 mt-0.5">
              <Building2 className="w-3.5 h-3.5 shrink-0" />
              {alumni.current_company || 'Independent'}
            </p>
            <p className="text-slate-300 text-[11px]">{alumni.current_job_title || 'Professional'}</p>
            {alumni.current_location && (
              <p className="text-slate-400 text-[10px] flex items-center gap-1 mt-1">
                <MapPin className="w-3 h-3 text-slate-500" />
                {alumni.current_location}
              </p>
            )}
          </div>
        </div>

        {/* Verification Status Banner */}
        <div
          className={`p-3 rounded-xl border flex items-center justify-between ${
            isVerified
              ? 'bg-emerald-950/30 border-emerald-800/40 text-emerald-300'
              : 'bg-amber-950/30 border-amber-800/40 text-amber-300'
          }`}
        >
          <div className="flex items-center gap-2">
            {isVerified ? <ShieldCheck className="w-4 h-4 text-emerald-400" /> : <Clock className="w-4 h-4 text-amber-400" />}
            <div>
              <span className="font-bold text-xs capitalize">{alumni.verification_status} Profile</span>
              {alumni.last_verified_at && (
                <span className="text-[10px] block text-slate-400">Verified: {alumni.last_verified_at}</span>
              )}
            </div>
          </div>
          <span className="text-[10px] font-mono px-2 py-0.5 rounded bg-slate-900 border border-slate-700">
            ID #{alumni.id}
          </span>
        </div>

        {/* Bio */}
        {alumni.bio && (
          <div>
            <h4 className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider mb-1.5">Biography</h4>
            <p className="text-slate-300 leading-relaxed bg-slate-900/60 p-3 rounded-xl border border-slate-800/80">
              {alumni.bio}
            </p>
          </div>
        )}

        {/* Academic Details */}
        <div>
          <h4 className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider mb-2 flex items-center gap-1.5">
            <GraduationCap className="w-3.5 h-3.5 text-purple-400" />
            Academic Credentials
          </h4>
          <div className="p-3 rounded-xl bg-slate-900/60 border border-slate-800/80 space-y-1.5">
            <div className="flex justify-between">
              <span className="text-slate-400">Institution:</span>
              <span className="text-slate-200 font-medium text-right">{alumni.institution_name || 'IIPS DAVV'}</span>
            </div>
            <div className="flex justify-between">
              <span className="text-slate-400">Department:</span>
              <span className="text-slate-200 font-medium text-right">{alumni.department_name}</span>
            </div>
            <div className="flex justify-between">
              <span className="text-slate-400">Degree & Batch:</span>
              <span className="text-purple-300 font-semibold">{alumni.degree} ({alumni.graduation_year})</span>
            </div>
          </div>
        </div>

        {/* Career Timeline */}
        {alumni.career_history?.length > 0 && (
          <div>
            <h4 className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider mb-2 flex items-center gap-1.5">
              <Briefcase className="w-3.5 h-3.5 text-cyan-400" />
              Employment History
            </h4>
            <div className="space-y-2 border-l-2 border-slate-800 ml-2 pl-3">
              {alumni.career_history.map((job) => (
                <div key={job.id} className="relative group">
                  <div className={`w-2.5 h-2.5 rounded-full absolute -left-[19px] top-1.5 ${job.is_current ? 'bg-cyan-400 ring-4 ring-cyan-950' : 'bg-slate-600'}`} />
                  <div className="p-2.5 rounded-xl bg-slate-900/80 border border-slate-800/80">
                    <div className="flex items-center justify-between mb-0.5">
                      <span className="font-bold text-white">{job.job_title}</span>
                      {job.is_current && (
                        <span className="text-[9px] uppercase px-1.5 py-0.2 rounded bg-cyan-950 text-cyan-300 border border-cyan-800/40">
                          Current
                        </span>
                      )}
                    </div>
                    <p className="text-cyan-400 text-[11px] font-medium">{job.company_name}</p>
                    <p className="text-[10px] text-slate-500 flex items-center gap-1 mt-1">
                      <Calendar className="w-3 h-3" />
                      {job.start_date} &rarr; {job.end_date || 'Present'}
                    </p>
                  </div>
                </div>
              ))}
            </div>
          </div>
        )}

        {/* Skills */}
        {alumni.skills?.length > 0 && (
          <div>
            <h4 className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider mb-2">Verified Skills</h4>
            <div className="flex flex-wrap gap-1.5">
              {alumni.skills.map((sk, idx) => (
                <span
                  key={idx}
                  className="px-2 py-1 rounded-md bg-slate-800/80 text-cyan-300 border border-slate-700/60 font-mono text-[10px]"
                >
                  {sk}
                </span>
              ))}
            </div>
          </div>
        )}

        {/* Confirmed Connections */}
        {alumni.connections?.length > 0 && (
          <div>
            <h4 className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider mb-2 flex items-center gap-1.5">
              <Users className="w-3.5 h-3.5 text-indigo-400" />
              Confirmed Connections ({alumni.connections.length})
            </h4>
            <div className="space-y-1.5">
              {alumni.connections.map((c) => (
                <div
                  key={c.id}
                  onClick={() => onSelectAlumniById && onSelectAlumniById(c.target_alumni_id)}
                  className="p-2 rounded-lg bg-slate-900/60 hover:bg-slate-800/80 border border-slate-800/80 hover:border-cyan-500/40 cursor-pointer flex items-center justify-between transition-all"
                >
                  <div>
                    <span className="text-white font-medium block">{c.target_name}</span>
                    <span className="text-[10px] text-slate-400">{c.target_company} &bull; {c.target_job_title}</span>
                  </div>
                  <span className="text-[9px] uppercase px-1.5 py-0.5 rounded bg-indigo-950 text-indigo-300 border border-indigo-800/40">
                    {c.relationship_type}
                  </span>
                </div>
              ))}
            </div>
          </div>
        )}

        {/* External Links */}
        <div className="pt-2">
          {alumni.linkedin_url && (
            <a
              href={alumni.linkedin_url}
              target="_blank"
              rel="noopener noreferrer"
              className="w-full py-2.5 px-3 rounded-xl bg-cyan-600 hover:bg-cyan-500 text-slate-950 font-bold text-xs flex items-center justify-center gap-2 shadow-lg shadow-cyan-500/20 transition-all mb-2"
            >
              <ExternalLink className="w-3.5 h-3.5 text-slate-950" />
              Open LinkedIn Profile Safely
            </a>
          )}
          {alumni.website_url && (
            <a
              href={alumni.website_url}
              target="_blank"
              rel="noopener noreferrer"
              className="w-full py-2 px-3 rounded-xl bg-slate-800 hover:bg-slate-700 text-slate-200 font-semibold text-xs flex items-center justify-center gap-2 border border-slate-700 transition-all"
            >
              <ExternalLink className="w-3.5 h-3.5" />
              Visit Personal Portfolio
            </a>
          )}
        </div>
      </div>
    </div>
  );
};

export default AlumniDetailPanel;
