/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id manager.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2014/06/23 14:58
 * @uses net manager class
 *       根据不同的服务器，可以继承该类来实现不同的需求
 */
#ifndef PF_NET_MANAGER_H_
#define PF_NET_MANAGER_H_

#include "pf/base/singleton.h"
#include "pf/sys/thread.h"
#include "pf/net/config.h"
#include "pf/net/connection/pool.h"
#if __LINUX__ && defined(_PF_NET_EPOLL) /* { */
#include "pf/net/connection/manager/epoll.h"
#elif __WINDOWS__ && defined(_PF_NET_IOCP) /* }{ */
#include "pf/net/connection/manager/iocp.h"
#else /* }{ */
#include "pf/net/connection/manager/select.h"
#endif /* } */
#include "pf/net/connection/base.h"
#include "pf/net/socket/server.h"

namespace pf_net {

#if __LINUX__ && defined(_PF_NET_EPOLL) /* { */
class PF_API Manager : public connection::manager::Epoll {
#elif __WINDOWS__ && defined(_PF_NET_IOCP) /* }{ */
class PF_API Manager : public connection::manager::Iocp {
#else /* }{ */
class PF_API Manager : public connection::manager::Select {
#endif /* } */

 public:
   Manager();
   ~Manager();

 public:
   virtual bool heartbeat();
   void loop();
   bool isactive();
   void setactive(bool active);

 public:
   //服务器广播
   void broadcast(packet::Base *packet);

 protected:
   bool active_;

};

}; //namespace pf_net

#endif //PF_NET_MANAGER_H_
