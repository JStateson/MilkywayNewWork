// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2018 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

// flags determining what is written to standard out.
// (errors go to stderr)
//
// NOTE: all writes to stdout should have an if (log_flags.*) {} around them.
//

#ifndef BOINC_CC_CONFIG_H
#define BOINC_CC_CONFIG_H

#include <vector>
#include <string>

#include "proxy_info.h"
#include "coproc.h"

#define DEFAULT_MAX_EVENT_LOG_LINES 2000

struct XML_PARSER;
struct PROJECT;
struct RESULT;

#define MAX_FILE_XFERS_PER_PROJECT      2
#define MAX_FILE_XFERS                  8
    // kind of arbitrary

struct LOG_FLAGS {
    // If you add anything, you must add it to parse() and write()

    // on by default; intended for all users
    //
    bool file_xfer;
        // file transfer start and finish
    bool sched_ops;
        // interactions with schedulers
    bool task;
        // task start and finish, and suspend/resume

    // off by default; intended for developers and testers
    //
    bool android_debug;
        // show Android-specific info (battery etc.)
    bool app_msg_receive;
        // show shared-mem message from apps
    bool app_msg_send;
        // show shared-mem message to apps
    bool async_file_debug;
        // show asynchronous file operations (copy, MD5, decompress)
    bool benchmark_debug;
        // debug CPU benchmarks
    bool checkpoint_debug;
    bool coproc_debug;
        // show coproc reserve/free and startup msgs
    bool cpu_sched;
        // preemption and resumption
    bool cpu_sched_debug;
        // explain scheduler decisions
    bool cpu_sched_status;
        // show what's running
    bool dcf_debug;
        // show changes to duration correction factors
    bool disk_usage_debug;
        // disk usage and project share info
    bool priority_debug;
        // info related to REC and scheduling priority
    bool file_xfer_debug;
        // show completion of FILE_XFER
    bool gui_rpc_debug;
    bool heartbeat_debug;
    bool http_debug;
    bool http_xfer_debug;
    bool idle_detection_debug;
        // show details leading to idle/not-idle determinations.
    bool mem_usage_debug;
        // memory usage
    bool network_status_debug;
    bool notice_debug;
    bool poll_debug;
        // show what polls are responding
    bool proxy_debug;
    bool rr_simulation;
        // results of RR sim
    bool rrsim_detail;
        // details of RR sim
    bool sched_op_debug;
	bool milkyway_debug; // jys debug flag for milkyway project
    bool scrsave_debug;
    bool slot_debug;
        // allocation of slots
    bool state_debug;
        // print textual summary of CLIENT_STATE initially
        // and after each scheduler RPC and garbage collect
        // also show actions of garbage collector
    bool statefile_debug;
        // show when and why state file is written
    bool suspend_debug;
        // details of processing and network suspend/resume
    bool task_debug;
        // task start and control details, and when apps checkpoint
    bool time_debug;
        // changes in on_frac, active_frac, connected_frac
    bool trickle_debug;
        // show trickle messages
    bool unparsed_xml;
        // show unparsed XML lines
    bool work_fetch_debug;
        // work fetch policy 
	bool mw_debug; //jys

    LOG_FLAGS();
    void init();
    int parse(XML_PARSER&);
    void show();
    int write(MIOFILE& out);
};

struct EXCLUDE_GPU {
    std::string url;
    std::string type;       // empty means all types
    std::string appname;    // empty means all apps
    int device_num;         // -1 means all instances

    int parse(XML_PARSER&);
    void write(MIOFILE&);
};

// if you add anything here, add it to
// defaults(), parse_options(), parse_options_client(), write(),
// and possibly show()
//
struct CC_CONFIG {
    bool abort_jobs_on_exit;
    bool allow_gui_rpc_get;
    bool allow_multiple_clients;
    bool allow_remote_gui_rpc;
    std::vector<std::string> alt_platforms;
    COPROCS config_coprocs;
    bool disallow_attach;
    bool dont_check_file_sizes;
    bool dont_contact_ref_site;
    bool dont_suspend_nci;
    bool dont_use_vbox;
    bool dont_use_wsl;
    std::vector<EXCLUDE_GPU> exclude_gpus;
    std::vector<std::string> exclusive_apps;
    std::vector<std::string> exclusive_gpu_apps;
    bool exit_after_finish;
    bool exit_before_start;
    bool exit_when_idle;
    bool fetch_minimal_work;
    bool fetch_on_update;
    std::string force_auth;
    bool http_1_0;
    int http_transfer_timeout_bps;
    int http_transfer_timeout;
    std::vector<int> ignore_gpu_instance[NPROC_TYPES];
    bool lower_client_priority;
    int max_event_log_lines;
    int max_file_xfers;
    int max_file_xfers_per_project;
    int max_stderr_file_size;
    int max_stdout_file_size;
    int max_tasks_reported;
    int ncpus;
    bool no_alt_platform;
    bool no_gpus;
    bool no_info_fetch;
    bool no_opencl;
    bool no_priority_change;
    bool os_random_only;
    int process_priority;
    int process_priority_special;
    PROXY_INFO proxy_info;
    double rec_half_life;
	int mw_wait_interval = 256;    // 0x100
	int mw_low_water_pct = 1;
	int mw_high_water_pct = 16; // hex 10
    bool report_results_immediately;
    bool run_apps_manually;
    int save_stats_days;
    bool skip_cpu_benchmarks;
    bool simple_gui_only;
    double start_delay;
    bool stderr_head;
    bool suppress_net_info;
    bool unsigned_apps_ok;
    bool use_all_gpus;
    bool use_certs;
    bool use_certs_only;
        // overrides use_certs
    bool vbox_window;

    CC_CONFIG();
    void defaults();
	int parse(FILE*);
	int parse(XML_PARSER&, LOG_FLAGS&);
    int parse_client(FILE*);
	int parse_options(XML_PARSER&);
    int parse_options_client(XML_PARSER&);
    int write(MIOFILE&, LOG_FLAGS&);
    void show();
};

//  Stuff related to app_config.xml

typedef std::vector<std::string> MSG_VEC;

struct APP_CONFIG {
    char name[256];
    int max_concurrent;
    double gpu_gpu_usage;
    double gpu_cpu_usage;
    bool fraction_done_exact;
    bool report_results_immediately;

    int parse(XML_PARSER&, MSG_VEC&, LOG_FLAGS&);
    int parse_gpu_versions(XML_PARSER&, MSG_VEC&, LOG_FLAGS&);
};

struct APP_VERSION_CONFIG {
    char app_name[256];
    char plan_class[256];
    char cmdline[256];
    double avg_ncpus;
    double ngpus;

    int parse(XML_PARSER&, MSG_VEC&, LOG_FLAGS&);
};

struct APP_CONFIGS {
    std::vector<APP_CONFIG> app_configs;
    std::vector<APP_VERSION_CONFIG> app_version_configs;
    int project_max_concurrent;
    bool project_has_mc;
        // have app- or project-level max concurrent restriction
    int project_min_mc;
        // the min of these restrictions
    bool report_results_immediately;

    int parse(XML_PARSER&, MSG_VEC&, LOG_FLAGS&);
    int parse_file(FILE*, MSG_VEC&, LOG_FLAGS&);
    int config_app_versions(PROJECT*, bool show_warnings);
    void write(MIOFILE&);
    void clear() {
        app_configs.clear();
        app_version_configs.clear();
        project_max_concurrent = 0;
        project_has_mc = false;
        project_min_mc = 0;
        report_results_immediately = false;
    }
};

#endif
