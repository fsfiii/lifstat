/*
 * lifstat.c
 * fsf - 2003-08-01
 *
 * Copyright (c) 2003, Frank S. Fejes III
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *  * Neither the name of the author nor the names of contributors may be
 *  used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "lifstat.h"

void usage_display(const char *name)
{
  printf("usage: %s [-nrmbdtzvh] [-i interface] [interval] [count]\n", name);
}

void version_display(void)
{
  printf("lifstat %s - written by Frank S. Fejes III\n", VERSION);
}

void header_display(int show_date, int show_time, int show_mb)
{
  if (show_date) { printf("      "); }
  if (show_time) { printf("         "); }
  printf("%-6s %10s %7s %5s %10s\n", "", "in", "", "", "out");
  if (show_date) { printf("date  "); }
  if (show_time) { printf("time     "); }
  printf("%-6s %10s %7s %5s ", "iface", show_mb ? "mbytes" : "bytes",
    "pkts", "err");
  printf("%10s %7s %5s %5s\n", show_mb ? "mbytes" : "bytes", "pkts",
    "err", "coll");
}

void header_display_bandwidth(int show_date, int show_time, int show_mb)
{
  if (show_date) { printf("      "); }
  if (show_time) { printf("         "); }
  printf("%-6s %9s %9s %9s\n", "", "in", "out", "total");
  if (show_date) { printf("date  "); }
  if (show_time) { printf("time     "); }
  printf("%-6s %9s ", "iface", show_mb ? "mbytes" : "kbytes");
  printf("%9s %9s %9s ", show_mb ? "mbytes" : "kbytes",
    show_mb ? "mbytes" : "kbytes", show_mb ? "MB/s" : "KB/s");
  printf("%5s %5s\n", "err", "coll");
}

int stats_display(char **ifaces, int ifc, stat_struct *cs,
  stat_struct *ps, int n, const char *ts, int show_mb, int bw_mode,
  int interval, int drop_zero)
{
  int i, x, ignore_iface;
  char *iface;
  long long in_bytes, in_pkts, tot_bytes;
  unsigned long in_errs;
  long long out_bytes, out_pkts;
  unsigned long out_errs;
  unsigned long coll;
  float in_f, out_f, tot_f;
  double total;
  int n_displayed = 0;

  for (i = 0; i < n; ++i) {
    if (strcmp(cs[i].iface, ps[i].iface) != 0) {
        continue;
    }

    iface = cs[i].iface;
    /* if the interface count is zero, display all */
    /* otherwise, only display designated interfaces */
    ignore_iface = 0;
    for (x = 0; x < ifc; ++x) {
        if (strcmp(ifaces[x], iface) != 0) {
            ignore_iface = 1;
        }
        else {
            ignore_iface = 0;
            break;
        }
    }
    if (ignore_iface) {
        continue;
    }

    in_bytes = cs[i].in_bytes - ps[i].in_bytes;
    in_pkts = cs[i].in_pkts - ps[i].in_pkts;
    in_errs = cs[i].in_errs - ps[i].in_errs;
    out_bytes = cs[i].out_bytes - ps[i].out_bytes;
    out_pkts = cs[i].out_pkts - ps[i].out_pkts;
    out_errs = cs[i].out_errs - ps[i].out_errs;
    coll = cs[i].coll - ps[i].coll;

    if (drop_zero) {
      if (show_mb) {
        total = ((in_bytes + out_bytes) / 1024.0 / 1024)
          + coll + in_errs + out_errs;
      }
      else {
        total = ((in_bytes + out_bytes) / 1024.0) + coll + in_errs + out_errs;
      }
      /* printf("%s : %lf\n", show_mb ? "mb" : "kb", total); */
      if (show_mb && (total < 0.1)) continue;
      if ((! show_mb) && (total < 1)) continue;
    }

    if ((! bw_mode) && show_mb) {
      in_f = in_bytes / 1024.0 / 1024.0;
      out_f = out_bytes / 1024.0 / 1024.0;

      if (ts) {
        printf("%s %-6s %10.1f %6lld %5lu ", ts, iface, in_f, in_pkts,
          in_errs);
      }
      else {
        printf("%-6s %10.1f %7lld %5lu ", iface, in_f, in_pkts, in_errs);
      }
      printf("%10.1f %7lld %5lu %5lu\n", out_f, out_pkts, out_errs, coll);
    }
    else if (! bw_mode) {
      if (ts) {
        printf("%s %-6s %10lld %7lld %5lu ", ts, iface, in_bytes,  in_pkts,
          in_errs);
      }
      else {
        printf("%-6s %10lld %7lld %5lu ", iface, in_bytes, in_pkts, in_errs);
      }
      printf("%10lld %7lld %5lu %5lu\n", out_bytes, out_pkts, out_errs, coll);
    }
    else if (show_mb) {
      tot_f = (in_bytes + out_bytes) / 1024.0 / 1024.0;
      in_f = in_bytes / 1024.0 / 1024.0;
      out_f = out_bytes / 1024.0 / 1024.0;

      if (ts) {
        printf("%s %-6s %9.1f ", ts, iface, in_f);
      }
      else {
        printf("%-6s %9.1f ", iface, in_f);
      }
      printf("%9.1f %9.1f %9.1f ", out_f, tot_f, tot_f / interval);
      printf("%5lu %5lu\n", in_errs + out_errs, coll);
    }
    else {
      /* bandwidth mode displays to kb */
      tot_bytes = (in_bytes + out_bytes) / 1024;
      in_bytes = in_bytes / 1024;
      out_bytes = out_bytes / 1024;

      if (ts) {
        printf("%s %-6s %9lld ", ts, iface, in_bytes);
      }
      else {
        printf("%-6s %9lld ", iface, in_bytes);
      }
      printf("%9lld %9lld %9lld ", out_bytes, tot_bytes, tot_bytes / interval);
      printf("%5lu %5lu\n", in_errs + out_errs, coll);
    }

    ++n_displayed;
  }

  return(n_displayed);
}

int read_line(int fd, char *line)
{
  char buf[1];
  int n = 0;

  while (read(fd, buf, 1) == 1) {
    if (buf[0] == '\n') {
      line[n] = 0;
      break;
    }
    line[n++] = buf[0];
  }

  line[n] = 0;

  return(n);
}

int line_parse(char *line, int len, stat_struct *stats)
{
  char *ptr = line;
  char *index;
  int i, n;
  char *cols[MAX_COLS];

  /* cut out leading spaces */
  for (i = 0; (i < len) && (line[i] == ' '); ++i, ++ptr);

  /* grab the interface name */
  index = strchr(ptr, ':');
  if (index) {
    *index = 0;
    strcpy(stats->iface, ptr);
    ptr = index + 1;
  }

  for (i = 0; i < MAX_COLS; ++i) {
    cols[i] = NULL;
  }

  n = split(ptr, cols, MAX_COLS, ' ');
  if (n < DATA_COLS) {
    fprintf(stderr, "Bad # of columns (%d instead of %d)!\n", n, DATA_COLS);
    return(-1);
  }

  stats->in_bytes = atoll(cols[0]);
  stats->in_pkts = atoll(cols[1]);
  stats->in_errs = strtoul(cols[3], (char **) NULL, 10);
  stats->out_bytes = atoll(cols[8]);
  stats->out_pkts = atoll(cols[9]);
  stats->out_errs = strtoul(cols[10], (char **) NULL, 10);
  stats->coll = strtoul(cols[13], (char **) NULL, 10);

  for (i = 0; i < MAX_COLS; ++i) {
    if (cols[i]) {
      free(cols[i]);
      cols[i] = NULL;
    }
  }

  return(0);
}

int stats_read(stat_struct *stats)
{
  int n_ifaces = 0;
  char line[MAX_LINE];
  char msg[256];
  FILE *f;

  f = fopen(DEVFILE, "r");
  if (! f) {
    sprintf(msg, "Cannot open file %s", DEVFILE);
    perror(msg);
  }

  /* throw out the first two lines */
  fgets(line, MAX_LINE, f);
  fgets(line, MAX_LINE, f);

  while (fgets(line, MAX_LINE, f) != NULL) {
    if (line_parse(line, strlen(line), &stats[n_ifaces]) != -1) {
      ++n_ifaces;
    }
  }

  if (ferror(f)) {
    sprintf(msg, "Error reading %s", DEVFILE);
    perror(msg);
  }

  fclose(f);

  return(n_ifaces);
}

void loop_display(char **ifaces, int ifc, int interval, int count,
  int show_date, int show_time, int show_mb, int repeat_header, int bw_mode,
  int drop_zero)
{
  stat_struct prev[MAX_IF];
  stat_struct curr[MAX_IF];
  int i = 0;
  int n;
  char *ts = NULL;
  time_t t;
  int n_displayed = 0;

  if (show_date || show_time) {
    ts = (char *) malloc(32 * sizeof(char));
  }

  if (count <= 0) {
    i = -1;
    count = 0;
  }

  while (i < count) {
    n = stats_read(prev);
    sleep(interval);
    n = stats_read(curr);

    if (ts) {
      t = time(NULL);
      if (show_date && show_time) {
        strftime(ts, 31, "%m-%d %H:%M:%S", localtime(&t));
      }
      else if (show_date) {
        strftime(ts, 31, "%m-%d", localtime(&t));
      }
      else {
        strftime(ts, 31, "%H:%M:%S", localtime(&t));
      }
    }

    n_displayed += stats_display(ifaces, ifc, curr, prev, n, ts, show_mb,
      bw_mode, interval, drop_zero);

    if (repeat_header && (n_displayed >= repeat_header)) {
      if (bw_mode) header_display_bandwidth(show_date, show_time, show_mb);
      else header_display(show_date, show_time, show_mb);
      n_displayed = 0;
    }

    if (i > -1) {
      ++i;
    }
  }
}

int main(int argc, char **argv)
{
  int interval = DEF_INTERVAL;
  int count = DEF_COUNT;
  int show_header = DEF_SHOW_HEADER;
  int show_time = DEF_SHOW_TIME;
  int show_date = DEF_SHOW_DATE;
  int show_mb = DEF_SHOW_MB;
  char *ifaces[MAX_IF];
  int ifc = 0;
  int ch;
  extern int optind;
  extern char *optarg;
  int repeat_header = 0;
  int bw_mode = 0;
  int drop_zero = 0;
 
  while ( (ch = getopt(argc, argv, "i:hvdtnmrbz")) != -1) {
    switch(ch) {
    case 'i':
      ifaces[ifc] = (char *) malloc(strlen(optarg) + 1);
      if (ifaces[ifc]) {
        strcpy(ifaces[ifc], optarg);
      }
      ++ifc;
      break;
    case 'n':
      show_header = 0;
      break;
    case 'm':
      show_mb = 1;
      break;
    case 'd':
      show_date = 1;
      break;
    case 't':
      show_time = 1;
      break;
    case 'r':
      repeat_header = DEF_REPEAT_HEADER;
      break;
    case 'b':
      bw_mode = 1;
      break;
    case 'z':
      drop_zero = 1;
      break;
    case 'h':
      usage_display(argv[0]);
      exit(0);
      break;
    case 'v':
      version_display();
      exit(0);
      break;
    }
  }
  argc -= optind;
  argv += optind; 

  if (argc) {
    interval = atoi(argv[0]);
  }
  if (argc > 1) {
    count = atoi(argv[1]);
  }

  if (show_header) {
    if (bw_mode) header_display_bandwidth(show_date, show_time, show_mb);
    else header_display(show_date, show_time, show_mb);
  }
  loop_display(ifaces, ifc, interval, count, show_date, show_time, show_mb,
    repeat_header, bw_mode, drop_zero);

  exit(0);
}

