/*
 *  Copyright 2020-present Daniel Trugman
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <iomanip>
#include <sstream>

#include "pfs/procfs.hpp"

template <typename T>
std::string join(const T& container)
{
    if (container.empty())
    {
        return "";
    }

    std::ostringstream out;
    for (const auto& v : container)
    {
        out << v << ',';
    }
    auto out_str = out.str();
    out_str.pop_back();
    return out_str;
}

bool is_printable(uint8_t byte)
{
    static const uint8_t PRINTABLE_MIN = 0x21;
    static const uint8_t PRINTABLE_MAX = 0x7e;

    return PRINTABLE_MIN <= byte && byte <= PRINTABLE_MAX;
}

std::string hexlify(const std::vector<uint8_t>& buffer)
{
    std::ostringstream out;
    out << std::hex;

    for (const auto& v : buffer)
    {
        char c = is_printable(v) ? static_cast<char>(v) : '.';
        out << c;
    }

    return out.str();
}

std::ostream& operator<<(std::ostream& out, const pfs::socket::timer timer)
{
    switch (timer)
    {
        case pfs::socket::timer::none:
            out << "None";
            break;

        case pfs::socket::timer::retransmit:
            out << "Retransmit";
            break;

        case pfs::socket::timer::another:
            out << "Another";
            break;

        case pfs::socket::timer::time_wait:
            out << "Time-Wait";
            break;

        case pfs::socket::timer::zero_window:
            out << "Zero-Window";
            break;

        default:
            out << "Unknown";
            break;
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::socket::state state)
{
    switch (state)
    {
        case pfs::socket::state::established:
            out << "Established";
            break;

        case pfs::socket::state::syn_sent:
            out << "Syn-Sent";
            break;

        case pfs::socket::state::syn_recv:
            out << "Syn-Recv";
            break;

        case pfs::socket::state::fin_wait1:
            out << "Fin-Wait1";
            break;

        case pfs::socket::state::fin_wait2:
            out << "Fin-Wait2";
            break;

        case pfs::socket::state::time_wait:
            out << "Time-Wait";
            break;

        case pfs::socket::state::close:
            out << "Close";
            break;

        case pfs::socket::state::close_wait:
            out << "Close-Wait";
            break;

        case pfs::socket::state::last_ack:
            out << "Last-Ack";
            break;

        case pfs::socket::state::listen:
            out << "Listen";
            break;

        case pfs::socket::state::closing:
            out << "Closing";
            break;

        default:
            out << "Unknown";
            break;
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::socket& socket)
{
    out << "slot[" << socket.slot << "] ";
    out << "local[" << socket.local_ip.to_string() << ":" << socket.local_port
        << "] ";
    out << "remote[" << socket.remote_ip.to_string() << ":"
        << socket.remote_port << "] ";
    out << "state[" << socket.current_state << "] ";
    out << "tx_queue[" << socket.tx_queue << "] ";
    out << "rx_queue[" << socket.rx_queue << "] ";
    out << "timer[" << socket.timer_active << "] ";
    out << "timer_expire[" << socket.timer_expire_jiffies << "] ";
    out << "retransmits[" << socket.retransmits << "] ";
    out << "uid[" << socket.uid << "] ";
    out << "timeouts[" << socket.timeouts << "] ";
    out << "inode[" << socket.inode << "] ";
    out << "ref_count[" << socket.ref_count << "] ";
    out << "skbuff[0x" << std::hex << socket.skbuff << std::dec << "] ";
    return out;
}

std::ostream& operator<<(std::ostream& out,
                         const pfs::unix_domain_socket::type type)
{
    switch (type)
    {
        case pfs::unix_domain_socket::type::stream:
            out << "Stream";
            break;

        case pfs::unix_domain_socket::type::datagram:
            out << "Datagram";
            break;

        case pfs::unix_domain_socket::type::seqpacket:
            out << "SeqPacket";
            break;

        default:
            out << "Unknown";
            break;
    }

    return out;
}

std::ostream& operator<<(std::ostream& out,
                         const pfs::unix_domain_socket::state state)
{
    switch (state)
    {
        case pfs::unix_domain_socket::state::free:
            out << "Free";
            break;

        case pfs::unix_domain_socket::state::unconnected:
            out << "Unconnected";
            break;

        case pfs::unix_domain_socket::state::connecting:
            out << "Connecting";
            break;

        case pfs::unix_domain_socket::state::connected:
            out << "Connected";
            break;

        case pfs::unix_domain_socket::state::disconnecting:
            out << "Disconnecting";
            break;

        default:
            out << "Unknown";
            break;
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::unix_domain_socket& uds)
{
    out << "skbuff[0x" << std::hex << uds.skbuff << std::dec << "] ";
    out << "ref_count[" << uds.ref_count << "] ";
    out << "protocol[" << uds.protocol << "] ";
    out << "flags[" << uds.flags << "] ";
    out << "type[" << uds.socket_type << "] ";
    out << "state[" << uds.socket_state << "] ";
    out << "inode[" << uds.inode << "] ";
    out << "path[" << uds.path << "] ";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::task_state state)
{
    switch (state)
    {
        case pfs::task_state::running:
            out << "Running";
            break;

        case pfs::task_state::sleeping:
            out << "Sleeping";
            break;

        case pfs::task_state::disk_sleep:
            out << "Disk-Sleep";
            break;

        case pfs::task_state::stopped:
            out << "Stopped";
            break;

        case pfs::task_state::tracing_stop:
            out << "Tracing-Stop";
            break;

        case pfs::task_state::zombie:
            out << "Zombie";
            break;

        case pfs::task_state::dead:
            out << "Dead";
            break;

        case pfs::task_state::wakekill:
            out << "Wake-Kill";
            break;

        case pfs::task_state::waking:
            out << "Waking";
            break;

        case pfs::task_state::parked:
            out << "Parked";
            break;

        case pfs::task_state::idle:
            out << "Idle";
            break;

        default:
            out << "Unknown";
            break;
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::status::uid_set& set)
{
    out << set.real << ',';
    out << set.effective << ',';
    out << set.saved_set << ',';
    out << set.filesystem;
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::status::seccomp seccomp)
{
    switch (seccomp)
    {
        case pfs::status::seccomp::disabled:
            out << "Disabled";
            break;

        case pfs::status::seccomp::strict:
            out << "Strict";
            break;

        case pfs::status::seccomp::filter:
            out << "Filter";
            break;

        default:
            out << "Unknown";
            break;
    }

    return out;
}

std::string to_octal_mask(uint64_t mask)
{
    std::ostringstream out;
    out << std::oct << std::setfill('0') << std::setw(4) << mask;
    return out.str();
}

std::string to_hex_mask(uint64_t mask)
{
    std::ostringstream out;
    out << std::hex << std::setfill('0') << std::setw(16) << mask;
    return out.str();
}

std::ostream& operator<<(std::ostream& out, const pfs::status& st)
{
    out << "name[" << st.name << "] ";
    out << "umask[" << to_octal_mask(st.umask) << "] ";
    out << "state[" << st.state << "] ";
    out << "tgid[" << st.tgid << "] ";
    out << "ngid[" << st.ngid << "] ";
    out << "pid[" << st.pid << "] ";
    out << "ppid[" << st.ppid << "] ";
    out << "tracer_pid[" << st.tracer_pid << "] ";
    out << "uid[" << st.uid << "] ";
    out << "gid[" << st.gid << "] ";
    out << "fdsize[" << st.fd_size << "] ";
    out << "groups[" << join(st.groups) << "] ";
    out << "ns_tgid[" << st.tgid << "] ";
    out << "ns_pid[" << st.pid << "] ";
    out << "ns_pgid[" << st.ns_pgid << "] ";
    out << "ns_sid[" << st.ns_sid << "] ";
    out << "vm_peak[" << st.vm_peak << "] ";
    out << "vm_size[" << st.vm_size << "] ";
    out << "vm_lck[" << st.vm_lck << "] ";
    out << "vm_pin[" << st.vm_pin << "] ";
    out << "vm_hwm[" << st.vm_hwm << "] ";
    out << "vm_rss[" << st.vm_rss << "] ";
    out << "rss_anon[" << st.rss_anon << "] ";
    out << "rss_file[" << st.rss_file << "] ";
    out << "rss_shmem[" << st.rss_shmem << "] ";
    out << "vm_data[" << st.vm_data << "] ";
    out << "vm_stk[" << st.vm_stk << "] ";
    out << "vm_exe[" << st.vm_exe << "] ";
    out << "vm_lib[" << st.vm_lib << "] ";
    out << "vm_pte[" << st.vm_pte << "] ";
    out << "vm_swap[" << st.vm_swap << "] ";
    out << "huge_tlb_pages[" << st.huge_tlb_pages << "] ";
    out << "core_dumping[" << std::boolalpha << st.core_dumping
        << std::noboolalpha << "] ";
    out << "threads[" << st.threads << "] ";
    out << "sig_q[" << st.sig_q.first << "/" << st.sig_q.second << "] ";
    out << "sig_pnd[" << to_hex_mask(st.sig_pnd.raw) << "] ";
    out << "shd_pnd[" << to_hex_mask(st.shd_pnd.raw) << "] ";
    out << "sig_blk[" << to_hex_mask(st.sig_blk.raw) << "] ";
    out << "sig_ign[" << to_hex_mask(st.sig_ign.raw) << "] ";
    out << "sig_cgt[" << to_hex_mask(st.sig_cgt.raw) << "] ";
    out << "cap_inh[" << to_hex_mask(st.cap_inh.raw) << "] ";
    out << "cap_prm[" << to_hex_mask(st.cap_prm.raw) << "] ";
    out << "cap_eff[" << to_hex_mask(st.cap_eff.raw) << "] ";
    out << "cap_bnd[" << to_hex_mask(st.cap_bnd.raw) << "] ";
    out << "cap_amb[" << to_hex_mask(st.cap_amb.raw) << "] ";
    out << "no_new_privs[" << std::boolalpha << st.no_new_privs
        << std::noboolalpha << "] ";
    out << "seccomp[" << st.seccomp_mode << "] ";
    out << "voluntary_ctxt_switches[" << st.voluntary_ctxt_switches << "] ";
    out << "nonvoluntary_ctxt_switches[" << st.nonvoluntary_ctxt_switches
        << "] ";

    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::stat& st)
{
    out << "pid[" << st.pid << "] ";
    out << "comm[" << st.comm << "] ";
    out << "state[" << st.state << "] ";
    out << "ppid[" << st.ppid << "] ";
    out << "pgrp[" << st.pgrp << "] ";
    out << "session[" << st.session << "] ";
    out << "tty_nr[" << st.tty_nr << "] ";
    out << "tgpid[" << st.tgpid << "] ";
    out << "flags[" << st.flags << "] ";
    out << "minflt[" << st.minflt << "] ";
    out << "cminflt[" << st.cminflt << "] ";
    out << "majflt[" << st.majflt << "] ";
    out << "cmajflt[" << st.cmajflt << "] ";
    out << "utime[" << st.utime << "] ";
    out << "stime[" << st.stime << "] ";
    out << "cutime[" << st.cutime << "] ";
    out << "cstime[" << st.cstime << "] ";
    out << "priority[" << st.priority << "] ";
    out << "nice[" << st.nice << "] ";
    out << "num_threads[" << st.num_threads << "] ";
    out << "itrealvalue[" << st.itrealvalue << "] ";
    out << "starttime[" << st.starttime << "] ";
    out << "vsize[" << st.vsize << "] ";
    out << "rss[" << st.rss << "] ";
    out << "rsslim[" << st.rsslim << "] ";
    out << "startcode[" << st.startcode << "] ";
    out << "endcode[" << st.endcode << "] ";
    out << "startstack[" << st.startstack << "] ";
    out << "kstkesp[" << st.kstkesp << "] ";
    out << "kstkeip[" << st.kstkeip << "] ";
    out << "signal[" << st.signal << "] ";
    out << "blocked[" << st.blocked << "] ";
    out << "sigignore[" << st.sigignore << "] ";
    out << "sigcatch[" << st.sigcatch << "] ";
    out << "wchan[" << st.wchan << "] ";
    out << "nswap[" << st.nswap << "] ";
    out << "cnswap[" << st.cnswap << "] ";
    out << "exit_signal[" << st.exit_signal << "] ";
    out << "processor[" << st.processor << "] ";
    out << "rt_priority[" << st.rt_priority << "] ";
    out << "policy[" << st.policy << "] ";
    out << "delayacct_blkio_ticks[" << st.delayacct_blkio_ticks << "] ";
    out << "guest_time[" << st.guest_time << "] ";
    out << "cguest_time[" << st.cguest_time << "] ";
    out << "start_data[" << st.start_data << "] ";
    out << "end_data[" << st.end_data << "] ";
    out << "start_brk[" << st.start_brk << "] ";
    out << "arg_start[" << st.arg_start << "] ";
    out << "arg_end[" << st.arg_end << "]";
    out << "env_start[" << st.env_start << "] ";
    out << "env_end[" << st.env_end << "]";
    out << "exit_code[" << st.exit_code << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::mem_stats& mem)
{
    out << "total[" << mem.total << "] ";
    out << "resident[" << mem.resident << "] ";
    out << "shared[" << mem.shared << "] ";
    out << "text[" << mem.text << "] ";
    out << "data[" << mem.data << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::mem_perm& perm)
{
    out << (perm.can_read ? 'r' : '-');
    out << (perm.can_write ? 'w' : '-');
    out << (perm.can_execute ? 'x' : '-');
    out << (perm.is_shared ? 's' : 'p');
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::mem_region& region)
{
    out << std::hex;
    out << "addr[0x" << region.start_address << "]-[0x" << region.end_address
        << "] ";
    out << "perm[" << region.perm << "]";
    out << "offset[0x" << region.offset << "] ";
    out << "device[" << region.device << "] ";
    out << std::dec;
    out << "inode[" << region.inode << "] ";
    out << "pathname[" << region.pathname << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::mount& mount)
{
    out << "id[" << mount.id << "] ";
    out << "parent_id[" << mount.parent_id << "] ";
    out << "device[" << mount.device << "] ";
    out << "root[" << mount.root << "] ";
    out << "point[" << mount.point << "] ";
    out << "options[" << join(mount.options) << "] ";
    out << "optional[" << join(mount.optional) << "] ";
    out << "fs[" << mount.filesystem_type << "] ";
    out << "source[" << mount.source << "] ";
    out << "super_options[" << join(mount.super_options) << "] ";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::module::state state)
{
    switch (state)
    {
        case pfs::module::state::live:
            out << "Live";
            break;

        case pfs::module::state::loading:
            out << "Loading";
            break;

        case pfs::module::state::unloading:
            out << "Unloading";
            break;

        default:
            out << "Unknown";
            break;
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::module& module)
{
    out << "name[" << module.name << "] ";
    out << "size[" << module.size << "] ";
    out << "instances[" << module.instances << "] ";
    out << "dependencies[" << join(module.dependencies) << "] ";
    out << "state[" << module.current_state << "] ";
    out << "offset[" << module.offset << "] ";
    out << std::boolalpha;
    out << "out_of_tree[" << module.is_out_of_tree << "] ";
    out << "unsigned[" << module.is_unsigned << "] ";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::load_average& load)
{
    out << "load[" << load.last_1min << ", " << load.last_5min << ", "
        << load.last_15min << "] ";
    out << "runnable_tasks[" << load.runnable_tasks << "] ";
    out << "total_tasks[" << load.total_tasks << "] ";
    out << "last_created_task[" << load.last_created_task << "] ";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pfs::zone& zone)
{
    out << "zone[" << zone.name << "] ";
    out << "chunks[" << join(zone.chunks) << "] ";
    return out;
}