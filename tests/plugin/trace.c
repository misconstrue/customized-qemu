/*
 * Copyright (C) 2018, Emilio G. Cota <cota@braap.org>
 *
 * License: GNU GPL, version 2 or later.
 *   See the COPYING file in the top-level directory.
 */
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <qemu-plugin.h>

QEMU_PLUGIN_EXPORT int qemu_plugin_version = QEMU_PLUGIN_VERSION;

char trace_file_name[100] = "trace.log";
FILE* ptrace_file = NULL;
uint64_t inst_cnt = 0;

/*
 * Trace TB translation callback.
 * This allows us to measure the overhead of injecting and then
 * removing trace instrumentation.
 */
static void vcpu_tb_trans(qemu_plugin_id_t id, struct qemu_plugin_tb *tb)
{
    size_t n = qemu_plugin_tb_n_insns(tb);
    size_t i;
    uint32_t opcode = 0x0;
    uint64_t vaddr = 0x0;
    const char* inst_disas;
    for(i=0; i<n; i++){
        struct qemu_plugin_insn *insn = qemu_plugin_tb_get_insn(tb, i);
        qemu_plugin_insn_data(insn, &opcode, sizeof(opcode));
        vaddr = qemu_plugin_insn_vaddr(insn);
        inst_disas = qemu_plugin_insn_disas(insn);
        fprintf(ptrace_file, "%ld clk (%ld) IT %lx %x: %s\n",\
                inst_cnt, inst_cnt, vaddr, opcode, inst_disas);
        inst_cnt++;
    }
}

static void plugin_exit(qemu_plugin_id_t id, void *p)
{
    fclose(ptrace_file);
}

QEMU_PLUGIN_EXPORT int qemu_plugin_install(qemu_plugin_id_t id,
                                           const qemu_info_t *info,
                                           int argc, char **argv)
{
    ptrace_file = fopen(trace_file_name, "w+");
    qemu_plugin_register_vcpu_tb_trans_cb(id, vcpu_tb_trans);
    qemu_plugin_register_atexit_cb(id, plugin_exit, NULL);
    return 0;
}
