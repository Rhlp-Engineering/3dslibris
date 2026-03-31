#pragma once

#include <cstddef>
#include <deque>

namespace browser_job_queue_utils {

template <typename JobT>
size_t PruneWarmupJobsForOtherBooks(std::deque<JobT> *jobs,
                                    const void *selected_book,
                                    int metadata_job_type,
                                    int cover_job_type) {
  if (!jobs)
    return 0;

  std::deque<JobT> kept;
  size_t removed = 0;
  while (!jobs->empty()) {
    JobT job = jobs->front();
    jobs->pop_front();
    const bool is_warmup =
        job.type == metadata_job_type || job.type == cover_job_type;
    if (is_warmup && job.book != selected_book) {
      removed++;
      continue;
    }
    kept.push_back(job);
  }
  jobs->swap(kept);
  return removed;
}

} // namespace browser_job_queue_utils
