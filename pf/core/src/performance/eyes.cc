#include "pf/base/util.h"
#include "pf/base/time_manager.h"
#include "pf/sys/process.h"
#include "pf/sys/info.h"
#include "pf/performance/eyes.h"

using namespace pf_performance;
Eyes *g_performance_eyes = NULL;

template <> Eyes *pf_base::Singleton<Eyes>::singleton_ = NULL;

Eyes *Eyes::getsingleton_pointer() {
  return singleton_;
}

Eyes &Eyes::getsingleton() {
  Assert(singleton_);
  return *singleton_;
}

Eyes::Eyes() {
  __ENTER_FUNCTION
    memset(sendbytes_, 0, sizeof(sendbytes_));
    memset(receivebytes_, 0, sizeof(receivebytes_));
    last_counttime_ = 0;
    last_printtime_ = 0;
    printinfo_interval_ = PERFORMANCE_EYES_PRINT_INTERVAL_DEFAULT;
  __LEAVE_FUNCTION
}

Eyes::~Eyes() {
  //do nothing
}

void Eyes::printinfo() {
  __ENTER_FUNCTION
    using namespace pf_sys;
    using namespace pf_base;
    int32_t current_processid = process::getid();
    char temp[128] = {0};
    info::loadaverage_t loadaverage;
    memset(&loadaverage, 0, sizeof(loadaverage));
    info::get_loadaverage(loadaverage);
    process::info_t processinfo;
    memset(&processinfo, 0, sizeof(processinfo));
    process::getinfo(current_processid, processinfo);
    DEBUGPRINTF("-------------------------------------------"
                "performance"
                "-------------------------------------------");
    //DEBUGPRINTF("- FPS: %.1f", FPF_);
    DEBUGPRINTF("- load average: %.2f, %.2f, %.2f", 
                loadaverage.oneminutes, 
                loadaverage.fiveminutes, 
                loadaverage.fifteenminutes);
    DEBUGPRINTF("- process id: %d", processinfo.id);
    DEBUGPRINTF("- process cpu usage: %.1f%%", processinfo.cpu_percent);
    util::get_sizestr(processinfo.VSZ, temp, sizeof(temp) - 1);
    DEBUGPRINTF("- process VSZ: %s", temp);
    util::get_sizestr(processinfo.RSS, temp, sizeof(temp) - 1);
    DEBUGPRINTF("- process RSS: %s", temp);
    DEBUGPRINTF("- online: %d", get_onlinecount());
    DEBUGPRINTF("- connection: %d", get_connectioncount());
    util::get_sizestr(get_uptraffic(), temp, sizeof(temp) - 1); 
    DEBUGPRINTF("- up traffic: %s", temp);
    util::get_sizestr(get_downtraffic(), temp, sizeof(temp) - 1);
    DEBUGPRINTF("- down traffic: %s", temp);
    char modulename[FILENAME_MAX] = {0};
    util::get_module_filename(modulename, sizeof(modulename));
    DEBUGPRINTF("- IMG: %s", modulename);
    DEBUGPRINTF("-------------------------------------------"
                "performance"
                "-------------------------------------------");
  __LEAVE_FUNCTION
}

void Eyes::activate() {
  __ENTER_FUNCTION
    using namespace pf_sys;
    using namespace pf_base;
    uint32_t currenttime = TIME_MANAGER_POINTER->get_current_time();
    if (currenttime - last_printtime_ >= printinfo_interval_ * 1000) {
      float cpu_usage = 0.0f;
      if (currenttime - last_printtime_ == currenttime) {
        int32_t current_processid = process::getid();
        cpu_usage = process::get_cpu_usage(current_processid);
      }
      if (cpu_usage != -1.0f) printinfo();
      last_printtime_ = currenttime;
    }
    //tick_forFPS();
    util::sleep(10 * 1000);
  __LEAVE_FUNCTION
}

void Eyes::tick_forFPS() {
  __ENTER_FUNCTION
    //计算自己的帧率毫无意义，应将主线程的帧率设置到变量中
    static uint32_t last_ticktime = 0;
    static uint32_t looptime = 0; //时间累计数
    static uint32_t loopcount = 0;
    uint32_t currenttime = TIME_MANAGER_POINTER->get_current_time();
    uint32_t difftime = currenttime - last_ticktime;
    //计算帧率
    const uint32_t kCalculateFPS = 1000; //每一秒计算一次
    if (difftime != currenttime) looptime += difftime;
    last_ticktime = currenttime;
    if (looptime > kCalculateFPS) {
      FPF_ = static_cast<float>((loopcount * 1000) / looptime);
      looptime = loopcount = 0;
    }
    ++loopcount;
  __LEAVE_FUNCTION
}

uint32_t Eyes::get_onlinecount() const {
  return onlinecount_;
}

uint32_t Eyes::get_connectioncount() const {
  return connectioncount_;
}
   
uint64_t Eyes::get_uptraffic() const {
  __ENTER_FUNCTION
    uint64_t result = sendbytes_[1] - sendbytes_[0];
    return result;
  __LEAVE_FUNCTION
    return 0;
}

uint64_t Eyes::get_downtraffic() const {
  __ENTER_FUNCTION
    uint64_t result = receivebytes_[1] - receivebytes_[0];
    return result;
  __LEAVE_FUNCTION
    return 0;
}

void Eyes::set_sendbytes(uint64_t bytes) {
  __ENTER_FUNCTION
    uint32_t currenttime = TIME_MANAGER_POINTER->get_current_time();
    if (currenttime - last_counttime_ > 60) {
      uint64_t temp = sendbytes_[1];
      sendbytes_[1] = bytes;
      sendbytes_[0] = temp;
      last_counttime_ = currenttime;
    }
  __LEAVE_FUNCTION
}

void Eyes::set_receivebytes(uint64_t bytes) {
  __ENTER_FUNCTION
    uint32_t currenttime = TIME_MANAGER_POINTER->get_current_time();
    if (currenttime - last_counttime_ > 60) {
      uint64_t temp = receivebytes_[1];
      receivebytes_[1] = bytes;
      receivebytes_[0] = temp;
      last_counttime_ = currenttime;
    }
  __LEAVE_FUNCTION
}

void Eyes::set_printinfo_interval(uint32_t interval) {
  printinfo_interval_ = interval;
}

void Eyes::set_onlinecount(uint32_t count) {
  onlinecount_ = count;
}

void Eyes::set_connectioncount(uint32_t count) {
  connectioncount_ = count;
}

void Eyes::set_fps(float value) {
  FPF_ = value;
}