/*!
 * \file batch.h
 * \brief The batch definition
 */
#pragma once

#include <unordered_map>
#include <vector>

#include "hpps/feeder/tensor.h"

namespace hpps {

class Batch {
 public:
  explicit Batch(const std::vector<std::string>& names);
  virtual ~Batch();

  Tensor* Get(const std::string& key);
  Tensor* Get(int index);
  void Set(int index, Tensor* tensor);
  std::vector<std::string> Keys();

 protected:
  std::unordered_map<std::string, int> index_;
  std::vector<Tensor*> tensors_;
};

}  // namespace hpps
