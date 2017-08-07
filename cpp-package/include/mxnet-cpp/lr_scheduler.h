/*!
*  Copyright (c) 2017 by Contributors
* \file lr_scheduler.h
* \brief Scheduling learning rate
*/

#ifndef MXNET_CPP_LR_SCHEDULER_H_
#define MXNET_CPP_LR_SCHEDULER_H_

#include "dmlc/logging.h"

namespace mxnet {
namespace cpp {

/*!
* \brief lr scheduler interface
*/
class LRScheduler {
 public:
  /*!
  * \brief constructor
  * \param base_lr the initial learning rate.
  */
  explicit LRScheduler(float base_lr = 0.01)
      : base_lr_(base_lr) {}
  /*!
  * \brief set base lr
  * \param lr learning rate from optimizer
  */
  void SetLR(const float lr) { base_lr_ = lr; }
  /*!
  * \brief get a new learning rate
  */
  virtual float GetLR(unsigned num_update) = 0;
  /*!
  * \brief destructor
  */
  virtual ~LRScheduler() {}

 protected:
  float base_lr_;
};

class FactorScheduler : public LRScheduler {
 public:
  explicit FactorScheduler(int step, float factor = 1, float stop_factor_lr = 1e-8)
      : LRScheduler() {
    step_ = step;
    factor_ = factor;
    stop_factor_lr_ = stop_factor_lr;
  }

  float GetLR(unsigned num_update) override {
    while (num_update > unsigned(count_ + step_)) {
      count_ += step_;
      base_lr_ *= factor_;
      if (base_lr_ < stop_factor_lr_) {
        base_lr_ = stop_factor_lr_;
        LG << "Update[" << num_update << "]: now learning rate arrived at " \
           << base_lr_ << ", will not change in the future";
      } else {
        LG << "Update[" << num_update << "]: Change learning rate to " << base_lr_;
      }
    }
    return base_lr_;
  }

 private:
  int count_ = 0;
  int step_;
  float factor_;
  float stop_factor_lr_;
};

}  // namespace cpp
}  // namespace mxnet

#endif  // MXNET_CPP_LR_SCHEDULER_H_