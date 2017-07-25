/**
* @version		GrPPI v0.2
* @copyright		Copyright (C) 2017 Universidad Carlos III de Madrid. All rights reserved.
* @license		GNU/GPL, see LICENSE.txt
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License in LICENSE.txt
* also available in <http://www.gnu.org/licenses/gpl.html>.
*
* See COPYRIGHT.txt for copyright notices and details.
*/

#ifndef GRPPI_NATIVE_PARALLEL_EXECUTION_NATIVE_H
#define GRPPI_NATIVE_PARALLEL_EXECUTION_NATIVE_H

#include <thread>
#include <atomic>
#include <algorithm>
#include <vector>
#include <type_traits>

#include "pool.h"
#include "../common/mpmc_queue.h"

namespace grppi {

//extern bool initialised;
//extern thread_pool pool;
/** @brief Set the execution mode to parallel with posix thread implementation 
 */
struct parallel_execution_native {
  public: 
  thread_pool pool;
  constexpr static int default_queue_size = 100;
  int queue_size = default_queue_size;
  queue_mode lockfree = queue_mode::blocking;

  void set_queue_size(int new_size){
     queue_size = new_size;
  }


  int get_thread_id(){
      while (lock.test_and_set(std::memory_order_acquire));
      auto it = std::find(thid_table.begin(), thid_table.end(), std::this_thread::get_id());
      auto id = std::distance(thid_table.begin(), it);
      lock.clear(std::memory_order_release);  
      return id; 
  }
  
  void register_thread(){
      while (lock.test_and_set(std::memory_order_acquire));
      thid_table.push_back(std::this_thread::get_id());    
      lock.clear(std::memory_order_release);  
  }
  
  void deregister_thread(){
      while (lock.test_and_set(std::memory_order_acquire));
      thid_table.erase(std::remove(thid_table.begin(), thid_table.end(),std::this_thread::get_id()), thid_table.end());
      lock.clear(std::memory_order_release);  
  }
  /** @brief Set num_threads to the maximum number of thread available by the
   *    hardware
   */
  parallel_execution_native(){ /*if(!initialised)*/ pool.initialise(this->num_threads); };

  /** @brief Set num_threads to _threads in order to run in parallel
   *
   *  @param _threads number of threads used in the parallel mode
   */
  parallel_execution_native(int _threads){ num_threads=_threads;  pool.initialise (_threads);};

  /** @brief Set num_threads to _threads in order to run in parallel and allows to disable the ordered execution
   *
   *  @param _threads number of threads used in the parallel mode
   *  @param _order enable or disable the ordered execution
   */
  parallel_execution_native(int _threads, bool order){ num_threads=_threads; ordering = order; pool.initialise (_threads);};

  /**
  \brief Set number of grppi threads.
  */
  void set_concurrency_degree(int degree) noexcept { num_threads = degree; }

  /**
  \brief Get number of grppi trheads.
  */
  int concurrency_degree() const noexcept { return num_threads; }

  /**
  \brief Enable ordering.
  */
  void enable_ordering() noexcept { ordering=true; }

  /**
  \brief Disable ordering.
  */
  void disable_ordering() noexcept { ordering=false; }

  /**
  \brief Is execution ordered.
  */
  bool is_ordered() const noexcept { return ordering; }


  private: 
     std::atomic_flag lock = ATOMIC_FLAG_INIT;
     std::vector<std::thread::id> thid_table;

  // TODO: Set default num_threads to hardware concurrency?
  constexpr static int default_num_threads = 4;
  int num_threads = default_num_threads;

  bool ordering = true;
};

/// Determine if a type is a threading execution policy.
template <typename E>
constexpr bool is_parallel_execution_native() {
  return std::is_same<E, parallel_execution_native>::value;
}

template <typename E>
constexpr bool is_supported();

template <>
constexpr bool is_supported<parallel_execution_native>() {
  return true;
}


} // end namespace grppi


#endif