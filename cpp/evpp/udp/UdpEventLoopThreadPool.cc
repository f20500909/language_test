// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/udp/UdpEventLoopThreadPool.h"

#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

UdpEventLoopThreadPool::UdpEventLoopThreadPool(const string& nameArg)
  : name_(nameArg),
    started_(false),
    numThreads_(0),
    next_(0)
{
}

UdpEventLoopThreadPool::~UdpEventLoopThreadPool()
{
  // Don't delete loop, it's stack variable
}

void UdpEventLoopThreadPool::start(const ThreadInitCallback& cb)
{
  assert(!started_);
  started_ = true;

  for (int i = 0; i < numThreads_; ++i)
  {
    char buf[name_.size() + 32];
    snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
    EventLoopThread* t = new EventLoopThread(cb, buf);
    threads_.push_back(std::unique_ptr<EventLoopThread>(t));
    loops_.push_back(t->startLoop());
  }
}

EventLoop* UdpEventLoopThreadPool::getNextLoop()
{
  assert(started_);
  EventLoop *loop =  nullptr;
  if (!loops_.empty())
  {
    // round-robin
    loop = loops_[next_];
    ++next_;
    if (implicit_cast<size_t>(next_) >= loops_.size())
    {
      next_ = 0;
    }
  }
  return loop;
}

EventLoop* UdpEventLoopThreadPool::getLoopForHash(size_t hashCode)
{
  EventLoop* loop = nullptr;

  if (!loops_.empty())
  {
    loop = loops_[hashCode % loops_.size()];
  }
  return loop;
}

std::vector<EventLoop*> UdpEventLoopThreadPool::getAllLoops()
{
  assert(started_);
  if (loops_.empty())
  {
    return std::vector<EventLoop*>();
  }
  else
  {
    return loops_;
  }
}
