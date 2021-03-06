/*!
 * \file feeder.h
 * \brief The feeder for i/o reading.
 */
#pragma once

#include <queue>

#include "hpps/common/thread_pool.h"
#include "hpps/feeder/plan.h"
#include "hpps/feeder/batch.h"
#include "hpps/feeder/blocking_queue.h"

namespace hpps {

class Feeder {
 public:
  static Feeder* Get() { static Feeder feeder; return &feeder; }

  void Start(int thread_num = 10);
  void Stop();

  // Schedule one data source's sample, the plan is managed by feeder.
  BlockingQueueIterator<std::unique_ptr<Batch>>* Schedule(
      Plan* plan, int max_queue_size = 1);

 protected:
  void Run(int tid);

  // Each entry mean one data source.
  struct Entry {
    Plan* plan;
    BlockingQueue<std::unique_ptr<Batch>>* blocking_queue;
  };
  void AddTask(const Entry& entry);

  // The task in data source.
  struct Task {
    Task() : curr(0) { }

    size_t curr;
    Plan::SampleRecord sample_record;
    BlockingQueue<std::unique_ptr<Batch>>* blocking_queue;
    // The cached BatchTensor
    std::queue<std::vector<Tensor*>>* cache_batch_tensor;
  };
  
  // Produce one batch
  void ProduceBatch(Queue<Task>* queue);
  // Assemble tensors into Batch 
  int AssembleBatch(Plan::SampleRecord& sample_record,
                    std::queue<std::vector<Tensor*>>& cache_batch_tensor,
                    std::unique_ptr<Batch>& batch);
  // Read batch tensor
  std::vector<std::vector<Tensor*>> ReadBatchTensor(Plan::SampleRecord& sample_record,
                                                    std::queue<std::vector<Tensor*>>& cache_batch_tensor);
  // Batch tensor to Batch
  void BatchTensor2Batch(std::vector<std::vector<Tensor*>>& batch_tensor,
                         Plan::SampleRecord& sample_record,
                         std::unique_ptr<Batch>& batch);

  ThreadPool* thread_pool_;
  std::vector<Entry> entries_;
  std::vector<Queue<Task>*> task_queues_;
  std::mutex mutex_;
  mutable bool running_ = false;
};

}  // namespace hpps
