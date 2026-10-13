import React, { useState } from 'react';
import { X, Route, ArrowRight, Sparkles, AlertCircle, Compass } from 'lucide-react';
import apiClient from '../api/client';

export const ShortestPathModal = ({ isOpen, onClose, alumniList = [], onHighlightPath }) => {
  const [sourceId, setSourceId] = useState(alumniList[0]?.id || 1);
  const [targetId, setTargetId] = useState(alumniList[4]?.id || 5);
  const [pathResult, setPathResult] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  if (!isOpen) return null;

  const handleFindPath = async () => {
    if (sourceId === targetId) {
      setError('Please select two distinct alumni members to find a path.');
      return;
    }
    setLoading(true);
    setError(null);
    try {
      const res = await apiClient.findShortestPath(sourceId, targetId);
      if (res.status === 'success') {
        setPathResult(res.data);
        if (onHighlightPath && res.data.pathNodeIds) {
          onHighlightPath(res.data.pathNodeIds);
        }
      } else {
        setError(res.message || 'Path calculation failed.');
      }
    } catch (err) {
      setError(err.message || 'Failed to query path from backend.');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="fixed inset-0 z-50 bg-black/75 backdrop-blur-sm flex items-center justify-center p-4">
      <div className="w-full max-w-xl glass-panel bg-[#0B101D] border border-cyan-500/40 rounded-2xl shadow-2xl p-6 text-xs text-slate-300">
        <div className="flex items-center justify-between pb-4 border-b border-slate-800">
          <div className="flex items-center gap-2.5">
            <div className="p-2 rounded-xl bg-cyan-950/80 border border-cyan-800/50">
              <Route className="w-5 h-5 text-cyan-400" />
            </div>
            <div>
              <h3 className="text-sm font-extrabold text-white">C++ Graph Shortest Path Discovery</h3>
              <p className="text-[11px] text-slate-400">Breadth-First Search (BFS) connection walk</p>
            </div>
          </div>
          <button
            onClick={onClose}
            className="p-1 rounded-lg text-slate-400 hover:text-white hover:bg-slate-800"
          >
            <X className="w-4 h-4" />
          </button>
        </div>

        {/* Source & Target Dropdowns */}
        <div className="grid grid-cols-1 sm:grid-cols-2 gap-4 my-5">
          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1">
              Source Alumni
            </label>
            <select
              value={sourceId}
              onChange={(e) => setSourceId(Number(e.target.value))}
              className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white focus:outline-none focus:border-cyan-400"
            >
              {alumniList.map((a) => (
                <option key={a.id} value={a.id}>
                  {a.full_name} ({a.current_company})
                </option>
              ))}
            </select>
          </div>

          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1">
              Target Alumni
            </label>
            <select
              value={targetId}
              onChange={(e) => setTargetId(Number(e.target.value))}
              className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white focus:outline-none focus:border-cyan-400"
            >
              {alumniList.map((a) => (
                <option key={a.id} value={a.id}>
                  {a.full_name} ({a.current_company})
                </option>
              ))}
            </select>
          </div>
        </div>

        {/* Error message */}
        {error && (
          <div className="p-3 mb-4 rounded-xl bg-rose-950/30 border border-rose-800/40 text-rose-300 flex items-center gap-2">
            <AlertCircle className="w-4 h-4 shrink-0 text-rose-400" />
            <span>{error}</span>
          </div>
        )}

        {/* Action Button */}
        <button
          onClick={handleFindPath}
          disabled={loading}
          className="w-full py-2.5 rounded-xl bg-gradient-to-r from-cyan-500 to-indigo-600 hover:from-cyan-400 hover:to-indigo-500 text-slate-950 font-bold text-xs flex items-center justify-center gap-2 shadow-lg shadow-cyan-500/25 transition-all disabled:opacity-50"
        >
          <Sparkles className="w-4 h-4" />
          {loading ? 'Traversing Graph Engine...' : 'Calculate Optimal Path (C++ BFS)'}
        </button>

        {/* Path Results */}
        {pathResult && (
          <div className="mt-5 p-4 rounded-xl bg-slate-900/80 border border-slate-800">
            {pathResult.found ? (
              <div>
                <div className="flex items-center justify-between mb-3 pb-2 border-b border-slate-800">
                  <span className="text-emerald-400 font-bold flex items-center gap-1.5">
                    <span className="w-2 h-2 rounded-full bg-emerald-400" />
                    Path Discovered!
                  </span>
                  <span className="text-slate-400 font-mono">
                    Degrees of Separation: <strong className="text-white">{pathResult.pathLength}</strong>
                  </span>
                </div>

                <div className="flex flex-col gap-2">
                  {pathResult.pathDetails?.map((node, idx) => (
                    <div key={node.numeric_id} className="flex items-center gap-2">
                      <div className="w-6 h-6 rounded-full bg-cyan-950 border border-cyan-800 text-cyan-400 font-bold flex items-center justify-center text-[10px]">
                        {idx + 1}
                      </div>
                      <div className="flex-1 p-2 rounded-lg bg-slate-950/60 border border-slate-800 flex items-center justify-between">
                        <div>
                          <span className="font-bold text-white block">{node.name}</span>
                          <span className="text-[10px] text-slate-400">{node.company} &bull; {node.job_title}</span>
                        </div>
                        <span className="text-[9px] font-mono px-1.5 py-0.5 rounded bg-slate-900 text-slate-400">
                          ID #{node.numeric_id}
                        </span>
                      </div>
                      {idx < pathResult.pathDetails.length - 1 && (
                        <ArrowRight className="w-3.5 h-3.5 text-cyan-400 shrink-0" />
                      )}
                    </div>
                  ))}
                </div>
              </div>
            ) : (
              <div className="text-center py-3 text-slate-400">
                <Compass className="w-8 h-8 mx-auto text-slate-600 mb-2" />
                <p className="font-semibold text-slate-300">{pathResult.message || 'No path exists.'}</p>
                <p className="text-[10px] text-slate-500 mt-1">These alumni reside in disconnected graph components.</p>
              </div>
            )}
          </div>
        )}
      </div>
    </div>
  );
};

export default ShortestPathModal;
