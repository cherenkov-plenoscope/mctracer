#include "mli_corsika_EventTape_standalone.h"



/* mli_version.c */
/* Copyright 2018-2020 Sebastian Achim Mueller */

void mli_logo_fprint(FILE *f)
{
        fprintf(f,
                "\n  "
                "                                        _/  _/              "
                "_/\n  "
                "   _/_/_/  _/_/      _/_/    _/  _/_/  _/        _/_/_/  "
                "_/_/_/_/\n  "
                "  _/    _/    _/  _/_/_/_/  _/_/      _/  _/  _/          "
                "_/\n  "
                " _/    _/    _/  _/        _/        _/  _/  _/          _/\n "
                " "
                "_/    _/    _/    _/_/_/  _/        _/  _/    _/_/_/      "
                "_/_/\n  "
                "\n");
}

void mli_authors_and_affiliations_fprint(FILE *f)
{
        fprintf(f,
                "  Sebastian Achim Mueller (1,2*,3^)\n"
                "\n"
                "  [1] Max-Planck-Institute for Nuclear Physics, \n"
                "      Saupfercheckweg 1, 69117 Heidelberg, Germany\n"
                "\n"
                "  [2] Institute for Particle Physics and Astrophysics,\n"
                "      ETH-Zurich, Otto-Stern-Weg 5, 8093 Zurich, Switzerland\n"
                "\n"
                "  [3] Experimental Physics Vb, Astroparticle Physics,\n"
                "      TU-Dortmund, Otto-Hahn-Str. 4a, 44227 Dortmund, "
                "Germany\n"
                "\n"
                "   *  (2015 - 2019)\n"
                "   ^  (2013 - 2015)\n");
}



/* mli_math.c */
/* Copyright 2018-2020 Sebastian Achim Mueller */

double mli_rad2deg(const double angle_in_rad)
{
        return 180. * angle_in_rad / MLI_PI;
}

double mli_deg2rad(const double angle_in_deg)
{
        return angle_in_deg * (1. / 180.) * MLI_PI;
}

double mli_hypot(const double a, const double b) { return sqrt(a * a + b * b); }

double mli_square(const double a) { return a * a; }

/*
 *  parameters
 *  ----------
 *      points          Sorted array in ascending order.
 *      num_points      Number of points.
 *      point_arg       The point to find the upper-bound for.
 */
uint64_t mli_upper_compare_double(
        const double *points,
        const uint64_t num_points,
        const double point_arg)
{
        uint64_t upper_index = 0;
        MLI_UPPER_COMPARE(points, num_points, point_arg, upper_index);
        return upper_index;
}

void mli_histogram(
        const double *bin_edges,
        const uint64_t num_bin_edges,
        uint64_t *underflow_bin,
        uint64_t *bins,
        uint64_t *overflow_bin,
        const double point)
{
        uint64_t idx_upper =
                mli_upper_compare_double(bin_edges, num_bin_edges, point);
        if (idx_upper == 0) {
                (*underflow_bin) += 1u;
        } else if (idx_upper == num_bin_edges) {
                (*overflow_bin) += 1u;
        } else {
                bins[idx_upper - 1] += 1u;
        }
}

void mli_linspace(
        const double start,
        const double stop,
        double *points,
        const uint64_t num_points)
{
        uint64_t i;
        const double range = stop - start;
        const double step = range / (double)(num_points - 1u);
        for (i = 0; i < num_points; i++) {
                points[i] = (double)i * step + start;
        }
}

double mli_mean(const double vals[], const uint64_t size)
{
        uint64_t i;
        double sum = 0;
        for (i = 0; i < size; i++) {
                sum = sum + vals[i];
        }
        return sum / (double)size;
}

double mli_std(const double vals[], const uint64_t size, const double vals_mean)
{
        uint64_t i;
        double s = 0.;
        for (i = 0; i < size; i++) {
                s = s + (vals[i] - vals_mean) * (vals[i] - vals_mean);
        }
        return sqrt(s / (double)size);
}

double mli_bin_center_in_linear_space(
        const double start,
        const double stop,
        const uint64_t num_bins,
        const uint64_t bin)
{
        const double width = stop - start;
        const double bin_width = width / (double)num_bins;
        return start + bin * bin_width + 0.5 * bin_width;
}

double mli_linear_interpolate_1d(
        const double weight,
        const double start,
        const double end)
{
        return start + weight * (end - start);
}

double mli_linear_interpolate_2d(
        const double xarg,
        const double x0,
        const double y0,
        const double x1,
        const double y1)
{
        /*
         *      |
         *  y1 -|            o
         *      |
         *  y0 -|    o
         *      |       xarg
         *      +----|---|---|----
         *          x0       x1
         *
         *  f(x) = m*x + b
         *  m = (y1 - y0)/(x1 - x0)
         *  y0 = m*x0 + b
         *  b = y0 - m*x0
         */
        const double m = (y1 - y0) / (x1 - x0);
        const double b = y0 - m * x0;
        return m * xarg + b;
}



/* mli_cstr.c */
/* Copyright 2018-2020 Sebastian Achim Mueller */

int mli_cstr_ends_with(const char *str, const char *sufix)
{
        uint64_t len_str, len_sufix;
        if (!str || !sufix) {
                return 0;
        }
        len_str = strlen(str);
        len_sufix = strlen(sufix);
        if (len_sufix > len_str) {
                return 0;
        }
        return strncmp(str + len_str - len_sufix, sufix, len_sufix) == 0;
}

int mli_cstr_starts_with(const char *str, const char *prefix)
{
        uint64_t len_str, len_prefix;
        if (!str || !prefix) {
                return 0;
        }
        len_str = strlen(str);
        len_prefix = strlen(prefix);
        if (len_prefix > len_str) {
                return 0;
        }
        return strncmp(str, prefix, len_prefix) == 0;
}

int mli_cstr_has_prefix_suffix(
        const char *str,
        const char *prefix,
        const char *sufix)
{
        uint64_t has_pre = 1;
        uint64_t has_suf = 1;
        if (prefix != NULL) {
                has_pre = mli_cstr_starts_with(str, prefix);
        }

        if (sufix != NULL) {
                has_suf = mli_cstr_ends_with(str, sufix);
        }

        if (has_pre == 1 && has_suf == 1) {
                return 1;
        } else {
                return 0;
        }
}

int mli_cstr_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length)
{
        uint64_t i = 0;
        memset(token, '\0', token_length);
        for (i = 0; i < token_length; i++) {
                if (str[i] == '\0') {
                        break;
                } else if (str[i] == delimiter) {
                        break;
                } else {
                        token[i] = str[i];
                }
        }
        return i;
}

int mli_cstr_is_CRLF(const char *s)
{
        if (s[0] == '\0') {
                return 0;
        }
        if (s[1] == '\0') {
                return 0;
        }
        if (s[0] == '\r' && s[1] == '\n') {
                return 1;
        }
        return 0;
}

int mli_cstr_is_CR(const char *s)
{
        if (s[0] == '\0') {
                return 0;
        }
        if (s[0] == '\r') {
                return 1;
        }
        return 0;
}

int mli_cstr_assert_only_NUL_LF_TAB_controls(const char *str)
{
        return mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(str, 1);
}

int mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(const char *str, const int dbg)
{
        uint64_t pos = 0;
        while (str[pos] != '\0') {
                if (str[pos] >= 32 && str[pos] < 127) {
                        /* all fine */
                } else {
                        if (str[pos] == '\n') {
                                /* fine */
                        } else if (str[pos] == '\t') {
                                /* fine */
                        } else {
                                if (dbg) {
                                        fprintf(stderr,
                                                "Control code %u "
                                                "at column %ld in string.\n",
                                                (uint8_t)str[pos],
                                                pos);
                                }
                                return 0;
                        }
                }
                pos += 1;
        }
        return 1;
}

uint64_t mli_cstr_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan)
{
        uint64_t i = 0;
        uint64_t count = 0u;
        while (str[i] != '\0' && i < num_chars_to_scan) {
                if (str[i] == c) {
                        count++;
                }
                i++;
        }
        return count;
}

int mli_fprint_line_match(
        FILE *f,
        const int64_t line,
        const int64_t line_number)
{
        chk(fprintf(f, "% 6d", (int32_t)line));
        if (line == line_number) {
                chk(fprintf(f, "->|  "));
        } else {
                chk(fprintf(f, "  |  "));
        }
        return 1;
error:
        return 0;
}

int mli_cstr_lines_fprint(
        FILE *f,
        const char *text,
        const uint64_t line_number,
        const uint64_t line_radius)
{
        int64_t _line_number = (int64_t)line_number;
        int64_t _line_radius = (int64_t)line_radius;
        int64_t line_start = MLI_MAX2(_line_number - _line_radius, 1);
        int64_t line_stop = line_number + line_radius;
        int64_t line = 1;
        int64_t i = 0;

        chk_msg(line_radius > 1, "Expected line_radius > 1.");

        chk(fprintf(f, "  line     text\n"));
        chk(fprintf(f, "        |\n"));

        while (text[i]) {
                int prefix = (line + 1 >= line_start) && (line < line_stop);
                int valid = (line >= line_start) && (line <= line_stop);
                if (text[i] == '\n') {
                        line++;
                }
                if (prefix && i == 0) {
                        chk(mli_fprint_line_match(f, line, _line_number));
                }
                if (valid) {
                        chk(putc(text[i], f));
                }
                if (prefix && text[i] == '\n') {
                        chk(mli_fprint_line_match(f, line, _line_number));
                }
                i++;
        }
        chk(putc('\n', f));

        return 1;
error:
        return 0;
}

void mli_cstr_path_strip_this_dir(char *dst, const char *src)
{
        const char *_src = &src[0];
        memset(dst, '\0', strlen(src));
        while (mli_cstr_starts_with(_src, "./") && _src[0] != '\0') {
                _src += 2;
        }
        strcpy(dst, _src);
}

void mli_cstr_path_basename_without_extension(const char *filename, char *key)
{
        uint64_t i = 0u;
        uint64_t o = 0u;

        while (1) {
                if (filename[i] == '\0') {
                        goto finalize;
                }
                if (filename[i] == '/') {
                        i += 1;
                        break;
                }
                i += 1;
        }

        while (1) {
                if (filename[i] == '\0') {
                        goto finalize;
                }
                if (filename[i] == '.') {
                        i += 1;
                        break;
                }
                key[o] = filename[i];
                i += 1;
                o += 1;
        }

finalize:
        key[o] = '\0';
}

void mli_cstr_strip_spaces(const char *in, char *out)
{
        uint64_t i = 0u;
        uint64_t o = 0u;
        while (in[i] && isspace(in[i])) {
                i += 1;
        }
        while (in[i] && !isspace(in[i])) {
                out[o] = in[i];
                i += 1;
                o += 1;
        }
        out[o] = '\0';
}

int mli_cstr_match_templeate(
        const char *s,
        const char *t,
        const char digit_wildcard)
{
        uint64_t i;
        if (strlen(s) != strlen(t)) {
                return 0;
        }
        for (i = 0; i < strlen(s); i++) {
                if (t[i] == digit_wildcard) {
                        if (!isdigit(s[i])) {
                                return 0;
                        }
                } else {
                        if (s[i] != t[i]) {
                                return 0;
                        }
                }
        }
        return 1;
}



/* mli_cstr_numbers.c */
/* Copyright 2018-2020 Sebastian Achim Mueller */

int mli_cstr_nto_int64(
        int64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t expected_num_chars)
{
        char *end;
        uint64_t actual_num_chars = 0u;
        int64_t l;
        chk_msg(!(s[0] == '\0' || isspace(s[0])),
                "Can not convert string to int64, bad string.");
        errno = 0;
        l = strtol(s, &end, base);
        chk_msg(errno != ERANGE,
                "Can not convert string to int64, over-, under-flow.");
        chk_msg(end != NULL, "Can not convert string to int64, bad string.");
        actual_num_chars = end - s;
        chk_msg(actual_num_chars == expected_num_chars,
                "Integer has not the expected number of chars.");
        *out = l;
        return 1;
error:
        return 0;
}

int mli_cstr_to_int64(int64_t *out, const char *s, const uint64_t base)
{
        chk_msg(mli_cstr_nto_int64(out, s, base, strlen(s)),
                "Can not convert string to int64.");
        return 1;
error:
        return 0;
}

int mli_cstr_nto_uint64(
        uint64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t expected_num_chars)
{
        int64_t tmp;
        chk(mli_cstr_nto_int64(&tmp, s, base, expected_num_chars));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return 1;
error:
        return 0;
}

int mli_cstr_to_uint64(uint64_t *out, const char *s, const uint64_t base)
{
        int64_t tmp;
        chk(mli_cstr_to_int64(&tmp, s, base));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return 1;
error:
        return 0;
}

int mli_cstr_nto_double(
        double *out,
        const char *s,
        const uint64_t expected_num_chars)
{
        char *end;
        uint64_t actual_num_chars = 0u;
        double l;
        chk_msg(!(s[0] == '\0' || isspace(s[0])),
                "Can not convert string to float64, bad string.");
        errno = 0;
        l = strtod(s, &end);
        chk_msg(errno != ERANGE,
                "Can not convert string to float64, over-, under-flow.");
        chk_msg(end != NULL, "Can not convert string to float64.");

        actual_num_chars = end - s;
        chk_msg(actual_num_chars == expected_num_chars,
                "float64 has not the expected number of chars.");
        *out = l;
        return 1;
error:
        return 0;
}

int mli_cstr_to_double(double *out, const char *s)
{
        chk_msg(mli_cstr_nto_double(out, s, strlen(s)),
                "Can not convert string to float64.");
        return 1;
error:
        return 0;
}

int mli_cstr_print_uint64(
        uint64_t u,
        char *s,
        const uint64_t max_num_chars,
        const uint64_t base,
        const uint64_t min_num_digits)
{
        char literals[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        char tmp[128] = {'\0'};
        uint64_t remainder = 0u;
        uint32_t remainder32 = 0u;
        uint64_t quotient = u;
        int64_t digs = 0u;
        int64_t pos = 0;
        int64_t i = 0;
        int64_t num_leading_zeors = 0;

        chk_msg(base <= 10, "Expected base <= 10");
        chk_msg(base > 1, "Expected base > 1");
        chk_msg(max_num_chars < sizeof(tmp), "Exceeded max num. chars.");
        chk_msg(min_num_digits < max_num_chars, "Exceeded max num. chars.");

        do {
                remainder = quotient % base;
                quotient = quotient / base;
                remainder32 = (uint32_t)remainder;
                tmp[digs] = literals[remainder32];
                digs++;
                chk_msg(digs < (int64_t)sizeof(tmp),
                        "Exceeded max num. chars.");
        } while (quotient > 0u);

        num_leading_zeors = min_num_digits - digs;
        if (num_leading_zeors < 0) {
                num_leading_zeors = 0;
        }

        for (i = 0; i < num_leading_zeors; i++) {
                chk_msg(pos < (int64_t)max_num_chars,
                        "Exceeded max num. chars.");
                s[pos] = '0';
                pos++;
        }

        for (i = 0; i < digs; i++) {
                chk_msg(pos < (int64_t)max_num_chars,
                        "Exceeded max num. chars.");
                s[pos] = tmp[digs - i - 1];
                pos++;
        }

        chk_msg(pos < (int64_t)max_num_chars, "Exceeded max num. chars.");
        s[pos] = '\0';

        return 1;
error:
        return 0;
}



/* mliTar.c */
/**
 * Copyright (c) 2017 rxi
 * Copyright (c) 2019 Sebastian A. Mueller
 *                    Max-Planck-Institute for nuclear-physics, Heidelberg
 */


struct mliTarRawHeader {
        char name[MLI_TAR_NAME_LENGTH];
        char mode[8];
        char owner[8];
        char group[8];
        char size[12];
        char mtime[12];
        char checksum[8];
        char type;
        char linkname[MLI_TAR_NAME_LENGTH];
        char _padding[255];
};

struct mliTar mliTar_init(void)
{
        struct mliTar out;
        out.stream = NULL;
        out.pos = 0u;
        out.remaining_data = 0u;
        return out;
}

struct mliTarHeader mliTarHeader_init(void)
{
        struct mliTarHeader h;
        h.mode = 0;
        h.owner = 0;
        h.size = 0;
        h.mtime = 0;
        h.type = 0;
        memset(h.name, '\0', sizeof(h.name));
        memset(h.linkname, '\0', sizeof(h.linkname));
        return h;
}

int mliTarHeader_set_normal_file(
        struct mliTarHeader *h,
        const char *name,
        const uint64_t size)
{
        (*h) = mliTarHeader_init();
        chk_msg(strlen(name) < sizeof(h->name), "Filename is too long.");
        memcpy(h->name, name, strlen(name));
        h->size = size;
        h->type = MLI_TAR_NORMAL_FILE;
        h->mode = 0664;
        return 1;
error:
        return 0;
}

int mliTarHeader_set_directory(struct mliTarHeader *h, const char *name)
{
        (*h) = mliTarHeader_init();
        chk_msg(strlen(name) < sizeof(h->name), "Dirname is too long.");
        memcpy(h->name, name, strlen(name));
        h->type = MLI_TAR_DIRECTORY;
        h->mode = 0775;
        return 1;
error:
        return 0;
}

/* write */

int mliTar_twrite(struct mliTar *tar, const void *data, const uint64_t size)
{
        int64_t res = fwrite(data, 1, size, tar->stream);
        chk_msg(res >= 0, "Failed writing to tar.");
        chk_msg((uint64_t)res == size, "Failed writing to tar.");
        tar->pos += size;
        return 1;
error:
        return 0;
}

/* read */

int mliTar_tread(struct mliTar *tar, void *data, const uint64_t size)
{
        int64_t res = fread(data, 1, size, tar->stream);
        chk_msg(res >= 0, "Failed reading from tar.");
        chk_msg((uint64_t)res == size, "Failed reading from tar.");
        tar->pos += size;
        return 1;
error:
        return 0;
}

/* close */

int mliTar_close(struct mliTar *tar)
{
        fclose(tar->stream);
        (*tar) = mliTar_init();
        return 1;
}

uint64_t mliTar_round_up(uint64_t n, uint64_t incr)
{
        return n + (incr - n % incr) % incr;
}

uint64_t mliTar_checksum(const struct mliTarRawHeader *rh)
{
        uint64_t i;
        unsigned char *p = (unsigned char *)rh;
        uint64_t res = 256;
        for (i = 0; i < offsetof(struct mliTarRawHeader, checksum); i++) {
                res += p[i];
        }
        for (i = offsetof(struct mliTarRawHeader, type); i < sizeof(*rh); i++) {
                res += p[i];
        }
        return res;
}

int mliTar_write_null_bytes(struct mliTar *tar, uint64_t n)
{
        uint64_t i;
        char nul = '\0';
        for (i = 0; i < n; i++) {
                chk_msg(mliTar_twrite(tar, &nul, 1), "Failed to write nulls");
        }
        return 1;
error:
        return 0;
}

int mliTar_field_to_uint(
        uint64_t *out,
        const char *field,
        const uint64_t field_size)
{
        char buff[MLI_TAR_NAME_LENGTH] = {'\0'};
        chk(field_size < MLI_TAR_NAME_LENGTH);
        memcpy(buff, field, field_size);

        /* Take care of historic 'space' (32 decimal) termination */
        /* Convert all 'space' terminations to '\0' terminations. */

        if (buff[field_size - 1] == 32) {
                buff[field_size - 1] = 0;
        }
        if (buff[field_size - 2] == 32) {
                buff[field_size - 2] = 0;
        }

        chk(mli_cstr_to_uint64(out, buff, MLI_TAR_OCTAL));
        return 1;
error:
        return 0;
}

int mliTar_raw_to_header(
        struct mliTarHeader *h,
        const struct mliTarRawHeader *rh)
{
        uint64_t chksum_actual, chksum_expected;
        chksum_actual = mliTar_checksum(rh);

        /* Build and compare checksum */
        chk_msg(mliTar_field_to_uint(
                        &chksum_expected, rh->checksum, sizeof(rh->checksum)),
                "bad checksum string.");
        chk_msg(chksum_actual == chksum_expected, "bad checksum.");

        /* Load raw header into header */
        chk_msg(mliTar_field_to_uint(&h->mode, rh->mode, sizeof(rh->mode)),
                "bad mode");
        chk_msg(mliTar_field_to_uint(&h->owner, rh->owner, sizeof(rh->owner)),
                "bad owner");
        if (rh->size[0] == -128) {
                chk_msg(mliTar_field12_to_uint64_2001star_base256(
                                rh->size, &h->size),
                        "bad size, mode: base-256");
        } else {
                chk_msg(mliTar_field_to_uint(
                                &h->size, rh->size, sizeof(rh->size)),
                        "bad size, mode: base-octal");
        }
        chk_msg(mliTar_field_to_uint(&h->mtime, rh->mtime, sizeof(rh->mtime)),
                "bad mtime");
        h->type = rh->type;
        memcpy(h->name, rh->name, sizeof(h->name));
        memcpy(h->linkname, rh->linkname, sizeof(h->linkname));

        return 1;
error:
        return 0;
}

int mliTar_uint_to_field(
        const uint64_t val,
        char *field,
        const uint64_t fieldsize)
{
        chk(mli_cstr_print_uint64(
                val, field, fieldsize, MLI_TAR_OCTAL, fieldsize - 1));
        return 1;
error:
        return 0;
}

int mliTar_uint64_to_field12_2001star_base256(uint64_t val, char *field)
{
        uint8_t tmp[12];
        int64_t i = 0;
        for (i = 11; i > 0; i--) {
                tmp[i] = (uint8_t)(val % 256u);
                val = val / 256u;
        }

        chk_msg(val == 0u, "Expected value to be less than 256**11.");
        /* set highest bit in leftmost byte to 1 */
        tmp[0] = (uint8_t)128u;

        memcpy(field, tmp, 12);
        return 1;
error:
        return 0;
}

int mliTar_field12_to_uint64_2001star_base256(const char *field, uint64_t *val)
{
        uint8_t tmp[12];
        uint64_t i = 0u;
        const uint64_t powers[] = {
                0x100000000000000,
                0x1000000000000,
                0x10000000000,
                0x100000000,
                0x1000000,
                0x10000,
                0x100,
                0x1,
        };

        memcpy(tmp, field, 12);
        chk_msg(tmp[0] == 128u,
                "Expected field[0] == 128, indicating 256-base, 2001star.");
        chk_msg(tmp[1] == 0u,
                "Expected field[1] == 0, 256**10 exceeds uint64.");
        chk_msg(tmp[2] == 0u,
                "Expected field[2] == 0, 256**09 exceeds uint64.");
        chk_msg(tmp[3] == 0u,
                "Expected field[3] == 0, 256**08 exceeds uint64.");

        (*val) = 0u;
        for (i = 4; i < 12; i++) {
                (*val) = (*val) + powers[i - 4] * (uint64_t)tmp[i];
        }
        return 1;
error:
        return 0;
}

int mliTar_make_raw_header(
        struct mliTarRawHeader *rh,
        const struct mliTarHeader *h)
{
        uint64_t chksum;

        /* Load header into raw header */
        memset(rh, 0, sizeof(*rh));
        chk_msg(mliTar_uint_to_field(h->mode, rh->mode, sizeof(rh->mode)),
                "bad mode");
        chk_msg(mliTar_uint_to_field(h->owner, rh->owner, sizeof(rh->owner)),
                "bad owner");
        if (h->size >= MLI_TAR_MAX_FILESIZE_OCTAL) {
                chk_msg(mliTar_uint64_to_field12_2001star_base256(
                                h->size, rh->size),
                        "bad size, mode: base-256");
        } else {
                chk_msg(mliTar_uint_to_field(
                                h->size, rh->size, sizeof(rh->size)),
                        "bad size, mode: base-octal");
        }
        chk_msg(mliTar_uint_to_field(h->mtime, rh->mtime, sizeof(rh->mtime)),
                "bad mtime");
        rh->type = h->type ? h->type : MLI_TAR_NORMAL_FILE;
        memcpy(rh->name, h->name, sizeof(rh->name));
        memcpy(rh->linkname, h->linkname, sizeof(rh->linkname));

        /* Calculate and write checksum */
        chksum = mliTar_checksum(rh);
        chk_msg(mli_cstr_print_uint64(
                        chksum,
                        rh->checksum,
                        sizeof(rh->checksum),
                        MLI_TAR_OCTAL,
                        sizeof(rh->checksum) - 2),
                "bad checksum");

        rh->checksum[sizeof(rh->checksum) - 1] = 32;

        chk_msg(rh->checksum[sizeof(rh->checksum) - 2] == 0,
                "Second last char in checksum must be '\\0', i.e. 0(decimal).");
        chk_msg(rh->checksum[sizeof(rh->checksum) - 1] == 32,
                "Last char in checksum must be ' ', i.e. 32(decimal).");

        return 1;
error:
        return 0;
}

int mliTar_open(struct mliTar *tar, const char *filename, const char *mode)
{
        *tar = mliTar_init();

        /* Assure mode is always binary */
        if (strchr(mode, 'r'))
                mode = "rb";
        if (strchr(mode, 'w'))
                mode = "wb";
        if (strchr(mode, 'a'))
                mode = "ab";

        tar->stream = fopen(filename, mode);
        chk_msg(tar->stream, "Failed to open tar-file.");

        return 1;
error:
        return 0;
}

int mliTar_raw_header_is_null(const struct mliTarRawHeader *rh)
{
        uint64_t i = 0u;
        unsigned char *p = (unsigned char *)rh;
        for (i = 0; i < sizeof(struct mliTarRawHeader); i++) {
                if (p[i] != '\0') {
                        return 0;
                }
        }
        return 1;
}

int mliTar_read_header(struct mliTar *tar, struct mliTarHeader *h)
{
        struct mliTarRawHeader rh;

        chk_msg(mliTar_tread(tar, &rh, sizeof(rh)),
                "Failed to read raw header");

        if (mliTar_raw_header_is_null(&rh)) {
                return 0;
        }

        chk_msg(mliTar_raw_to_header(h, &rh), "Failed to parse raw header.");
        tar->remaining_data = h->size;
        return 1;
error:
        return 0;
}

int mliTar_read_data(struct mliTar *tar, void *ptr, uint64_t size)
{
        chk_msg(tar->remaining_data >= size,
                "Expect size to be read >= remaining_data");
        chk_msg(mliTar_tread(tar, ptr, size), "Failed to read payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                uint64_t i;
                const uint64_t next_record = mliTar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;
                char padding;

                for (i = 0; i < padding_size; i++) {
                        chk_msg(mliTar_tread(tar, &padding, 1),
                                "Failed to read padding-block "
                                "to reach next record.");
                }
        }

        return 1;
error:
        return 0;
}

int mliTar_write_header(struct mliTar *tar, const struct mliTarHeader *h)
{
        struct mliTarRawHeader rh;
        chk_msg(mliTar_make_raw_header(&rh, h), "Failed to make raw-header");
        tar->remaining_data = h->size;
        chk_msg(mliTar_twrite(tar, &rh, sizeof(rh)), "Failed to write header.");
        return 1;
error:
        return 0;
}

int mliTar_write_data(struct mliTar *tar, const void *data, uint64_t size)
{
        chk_msg(tar->remaining_data >= size,
                "Expect tar->remaining_data >= size to be written.");
        chk_msg(mliTar_twrite(tar, data, size),
                "Failed to write payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                const uint64_t next_record = mliTar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;
                chk_msg(mliTar_write_null_bytes(tar, padding_size),
                        "Failed to write padding zeros.");
        }
        return 1;
error:
        return 0;
}

int mliTar_finalize(struct mliTar *tar)
{
        chk_msg(mliTar_write_null_bytes(
                        tar, sizeof(struct mliTarRawHeader) * 2),
                "Failed to write two final null records.");
        return 1;
error:
        return 0;
}



/* mliDynArray.c */
/* Copyright 2018-2020 Sebastian Achim Mueller */



/* mliDynFloat.c */
/* Copyright 2018-2020 Sebastian Achim Mueller */
MLIDYNARRAY_IMPLEMENTATION(mli, Float, float)



/* mli_corsika_utils.c */
/* Copyright 2020 Sebastian A. Mueller*/

float mli_chars_to_float(const char *four_char_word)
{
        float f;
        memcpy(&f, four_char_word, sizeof(float));
        return f;
}



/* mli_corsika_EventTape.c */
/* Copyright 2020 Sebastian A. Mueller */

/* writer */
/* ====== */
struct mliEventTapeWriter mliEventTapeWriter_init(void)
{
        struct mliEventTapeWriter tio;
        tio.tar = mliTar_init();
        tio.run_number = 0;
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 1;
        tio.buffer = mliDynFloat_init();
        return tio;
}

int mliEventTapeWriter_close(struct mliEventTapeWriter *tio)
{
        if (tio->tar.stream) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
                chk_msg(mliTar_finalize(&tio->tar), "Can't finalize tar-file.");
                chk_msg(mliTar_close(&tio->tar), "Can't close tar-file.");
        }
        mliDynFloat_free(&tio->buffer);
        (*tio) = mliEventTapeWriter_init();
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_open(
        struct mliEventTapeWriter *tio,
        const char *path,
        const uint64_t num_bunches_buffer)
{
        chk_msg(mliEventTapeWriter_close(tio),
                "Can't close and free previous tar-io-writer.");
        chk_msg(mliTar_open(&tio->tar, path, "w"), "Can't open tar.");
        chk_msg(mliDynFloat_malloc(&tio->buffer, 8 * num_bunches_buffer),
                "Can't malloc cherenkov-bunch-buffer.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_corsika_header(
        struct mliEventTapeWriter *tio,
        const char *path,
        const float *corsika_header)
{
        struct mliTarHeader tarh = mliTarHeader_init();
        chk_msg(mliTarHeader_set_normal_file(
                        &tarh, path, MLI_CORSIKA_HEADER_SIZE_BYTES),
                "Can't set tar-header for corsika-header.");
        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for corsika-header to tar.");
        chk_msg(mliTar_write_data(
                        &tio->tar,
                        corsika_header,
                        MLI_CORSIKA_HEADER_SIZE_BYTES),
                "Can't write data of corsika-header to tar.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_runh(
        struct mliEventTapeWriter *tio,
        const float *runh)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        tio->run_number = (int)(MLI_ROUND(runh[1]));
        chk_msg(tio->run_number >= 1, "Expected run_number >= 1.");
        sprintf(path, "%09d/RUNH.float32", tio->run_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, runh),
                "Can't write 'RUNH.float32' to event-tape.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_evth(
        struct mliEventTapeWriter *tio,
        const float *evth)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        int evth_run_number = (int)(
                MLI_ROUND(evth[MLI_CORSIKA_EVTH_RUN_NUMBER]));

        if (tio->event_number > 0) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
        }
        chk_msg(tio->run_number != 0, "Expected RUNH before EVTH.");
        chk_msg(tio->run_number == evth_run_number,
                "Expected run_number in EVTH "
                "to match run_number in last RUNH.");
        tio->event_number = (int)(
                MLI_ROUND(evth[MLI_CORSIKA_EVTH_EVENT_NUMBER]));
        chk_msg(tio->event_number > 0, "Expected event_number > 0.");
        tio->cherenkov_bunch_block_number = 1;
        sprintf(path,
                "%09d/%09d/EVTH.float32",
                tio->run_number,
                tio->event_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, evth),
                "Can't write 'EVTH.float32' to event-tape.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_flush_cherenkov_bunch_block(
        struct mliEventTapeWriter *tio)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        struct mliTarHeader tarh = mliTarHeader_init();
        sprintf(path,
                "%09d/%09d/%09d.cer.x8.float32",
                tio->run_number,
                tio->event_number,
                tio->cherenkov_bunch_block_number);
        chk_msg(mliTarHeader_set_normal_file(
                        &tarh, path, tio->buffer.size * sizeof(float)),
                "Can't set cherenkov-bunch-block's tar-header.");
        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for cherenkov-bunch-block to tar.");
        chk_msg(mliTar_write_data(
                        &tio->tar,
                        tio->buffer.array,
                        tio->buffer.size * sizeof(float)),
                "Can't write cherenkov-bunch-block to tar-file.");
        tio->buffer.size = 0;
        tio->cherenkov_bunch_block_number += 1;
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_cherenkov_bunch(
        struct mliEventTapeWriter *tio,
        const float *bunch)
{
        uint64_t i;
        if (tio->buffer.size == tio->buffer.capacity) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
        }
        for (i = 0; i < 8; i ++) {
            tio->buffer.array[tio->buffer.size] = bunch[i];
            tio->buffer.size += 1;
        }
        return 1;
error:
        return 0;
}

/* reader */
/* ====== */

struct mliEventTapeReader mliEventTapeReader_init(void)
{
        struct mliEventTapeReader tio;
        tio.tar = mliTar_init();
        tio.tarh = mliTarHeader_init();
        tio.run_number = 0;
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 0;
        tio.block_at = 0;
        tio.block_size = 0;
        tio.has_still_bunches_in_event = 0;
        return tio;
}

int mliEventTapeReader_close(struct mliEventTapeReader *tio)
{
        if (tio->tar.stream) {
                chk_msg(mliTar_close(&tio->tar), "Can't close tar-file.");
        }
        (*tio) = mliEventTapeReader_init();
        return 1;
error:
        return 0;
}

int mliEventTapeReader_open(struct mliEventTapeReader *tio, const char *path)
{
        chk_msg(mliEventTapeReader_close(tio),
                "Can't close and free previous tar-io-reader.");
        chk_msg(mliTar_open(&tio->tar, path, "r"), "Can't open tar.");
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_runh(struct mliEventTapeReader *tio, float *runh)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t runh_run_number = 0;
        chk_msg(tio->has_tarh, "Expected next tar-header.");
        chk_msg(mli_cstr_match_templeate(
                        tio->tarh.name,
                        "ddddddddd/RUNH.float32",
                        'd'),
                "Expected file to be 'ddddddddd/RUNH.float32.'");
        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected RUNH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)runh, tio->tarh.size),
                "Can't read RUNH from tar.");
        chk_msg(runh[0] == mli_chars_to_float("RUNH"),
                "Expected RUNH[0] == 'RUNH'");
        chk_msg(mli_cstr_nto_uint64(
                        &tio->run_number,
                        &tio->tarh.name[0],
                        BASE,
                        NUM_DIGITS),
                "Can't read run_number from RUNH's path.");
        runh_run_number = (uint64_t)(
            MLI_ROUND(runh[MLI_CORSIKA_RUNH_RUN_NUMBER]));
        chk_msg(tio->run_number == runh_run_number,
                "Expected run_number in RUNH's path "
                "to match run_number in RUNH.");
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_evth(struct mliEventTapeReader *tio, float *evth)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t path_event_number;
        uint64_t evth_event_number;
        uint64_t path_run_number;
        uint64_t evth_run_number;
        char match[MLI_TAR_NAME_LENGTH] = "ddddddddd/ddddddddd/EVTH.float32";

        if (!tio->has_tarh) {
                return 0;
        }
        if (!mli_cstr_match_templeate(tio->tarh.name, match, 'd')) {
                return 0;
        }
        chk_msg(mli_cstr_nto_uint64(
                        &path_event_number,
                        &tio->tarh.name[10],
                        BASE,
                        NUM_DIGITS),
                "Can't parse event-number from path.");
        chk_msg(mli_cstr_nto_uint64(
                        &path_run_number,
                        &tio->tarh.name[0],
                        BASE,
                        NUM_DIGITS),
                "Can't parse run-number from path.");
        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected EVTH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)evth, tio->tarh.size),
                "Can't read EVTH from tar.");
        chk_msg(evth[0] == mli_chars_to_float("EVTH"),
                "Expected EVTH[0] == 'EVTH'");

        evth_event_number = (uint64_t)evth[MLI_CORSIKA_EVTH_EVENT_NUMBER];
        evth_run_number = (uint64_t)evth[MLI_CORSIKA_EVTH_RUN_NUMBER];

        chk_msg(evth_event_number == path_event_number,
                "Expected paths' event-number to match event-number in EVTH.");
        chk_msg(evth_run_number == path_run_number,
                "Expected paths' run-number to match run-number in EVTH.");

        tio->event_number = evth_event_number;
        tio->cherenkov_bunch_block_number = 1;

        /* now there must follow a cherenkov-bunch-block */
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);

        chk_msg(tio->has_tarh, "Expected cherenkov-bunch-block after EVTH.");
        chk_msg(mliEventTapeReader_tarh_is_valid_cherenkov_block(tio),
                "Cherenkov-bunch-block's tar-header doesn't match.");

        chk_msg(tio->tarh.size % MLI_CORSIKA_BUNCH_SIZE_BYTES == 0,
                "Expected cherenkov-bunch-block-size "
                "to be multiple of bunch-size.");
        tio->block_size = tio->tarh.size / MLI_CORSIKA_BUNCH_SIZE_BYTES;
        tio->block_at = 0;
        tio->has_still_bunches_in_event = 1;

        return 1;
error:
        return 0;
}

int mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
        const struct mliEventTapeReader *tio)
{
        char match[MLI_TAR_NAME_LENGTH] =
                "ddddddddd/ddddddddd/ddddddddd.cer.x8.float32";
        return mli_cstr_match_templeate(tio->tarh.name, match, 'd');
}

int mliEventTapeReader_tarh_is_valid_cherenkov_block(
        const struct mliEventTapeReader *tio)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t path_run_number;
        uint64_t path_event_number;
        uint64_t path_block_number;
        chk_msg(tio->has_tarh, "Expected a next tar-header.");

        chk_msg(mliEventTapeReader_tarh_might_be_valid_cherenkov_block(tio),
                "Expected cherenkov-bunch-block-name to be valid.");

        chk_msg(mli_cstr_nto_uint64(
                        &path_run_number,
                        &tio->tarh.name[0],
                        BASE,
                        NUM_DIGITS),
                "Can't parse run-number from path.");
        chk_msg(path_run_number == tio->run_number,
                "Expected consistent run-number in cherenkov-block-path.");

        chk_msg(mli_cstr_nto_uint64(
                        &path_event_number,
                        &tio->tarh.name[9 + 1],
                        BASE,
                        NUM_DIGITS),
                "Can't parse event-number from path.");
        chk_msg(path_event_number == tio->event_number,
                "Expected same event-number in cherenkov-block-path and EVTH.");

        chk_msg(mli_cstr_nto_uint64(
                        &path_block_number,
                        &tio->tarh.name[9 + 1 + 9 + 1],
                        BASE,
                        NUM_DIGITS),
                "Can't parse cherenkov-block-number from path.");
        chk_msg(path_block_number == tio->cherenkov_bunch_block_number,
                "Expected different cherenkov-bunch-block-number in "
                "cherenkov-block-path.");
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_cherenkov_bunch(
        struct mliEventTapeReader *tio,
        float *bunch)
{
        if (tio->has_still_bunches_in_event == 0) {
                return 0;
        }
        if (tio->block_at == tio->block_size) {
                tio->cherenkov_bunch_block_number += 1;
                tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
                if (!tio->has_tarh) {
                        tio->has_still_bunches_in_event = 0;
                        return 0;
                }
                if (!mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
                            tio)) {
                        tio->has_still_bunches_in_event = 0;
                        return 0;
                }
                chk_msg(mliEventTapeReader_tarh_is_valid_cherenkov_block(tio),
                        "Cherenkov-bunch-block's tar-header doesn't match.");
                chk_msg(tio->tarh.size % MLI_CORSIKA_BUNCH_SIZE_BYTES == 0,
                        "Expected cherenkov-bunch-block-size "
                        "to be multiple of bunch-size.");
                tio->block_size = tio->tarh.size / MLI_CORSIKA_BUNCH_SIZE_BYTES;
                tio->block_at = 0;
        }
        chk_msg(mliTar_read_data(
                        &tio->tar,
                        (void *)(bunch),
                        MLI_CORSIKA_BUNCH_SIZE_BYTES),
                "Failed to read cherenkov_bunch.");

        tio->block_at += 1;
        return 1;
error:
        return 0;
}



