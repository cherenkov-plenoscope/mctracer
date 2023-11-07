#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stddef.h>
#include <math.h>



/* chk_debug.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef CHK_DEBUG_H_
#define CHK_DEBUG_H_


/*
 *  Based on Zed Shawn's awesome Debug Macros from his book:
 *  Learn C the hard way
 */

#define chk_clean_errno() (errno == 0 ? "None" : strerror(errno))
#define chk(C) chk_msg(C, "Not expected.")
#define chk_mem(C) chk_msg((C), "Out of memory.")

#define chk_eprint(MSG)                                                        \
        fprintf(stderr,                                                        \
                "[ERROR] (%s:%d: errno: %s) " MSG "\n",                        \
                __FILE__,                                                      \
                __LINE__,                                                      \
                chk_clean_errno())

#define chk_msg(C, MSG)                                                        \
        if (!(C)) {                                                            \
                chk_eprint(MSG);                                               \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define chk_bad(MSG)                                                           \
        {                                                                      \
                chk_eprint(MSG);                                               \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define chk_malloc(PTR, TYPE, NUM)                                             \
        {                                                                      \
                PTR = (TYPE *)malloc(NUM * sizeof(TYPE));                      \
                chk_mem(PTR);                                                  \
        }

#define chk_fwrite(PTR, SIZE_OF_TYPE, NUM, F)                                  \
        {                                                                      \
                const uint64_t num_written =                                   \
                        fwrite(PTR, SIZE_OF_TYPE, NUM, F);                     \
                chk_msg(num_written == NUM, "Can not write to file.");         \
        }

#define chk_fread(PTR, SIZE_OF_TYPE, NUM, F)                                   \
        {                                                                      \
                const uint64_t num_read = fread(PTR, SIZE_OF_TYPE, NUM, F);    \
                chk_msg(num_read == NUM, "Can not read from file.");           \
        }

#endif



/* mli_version.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VERSION_H_
#define MLI_VERSION_H_


#define MLI_VERSION_MAYOR 1
#define MLI_VERSION_MINOR 5
#define MLI_VERSION_PATCH 2

void mli_logo_fprint(FILE *f);
void mli_authors_and_affiliations_fprint(FILE *f);
#endif



/* mli_math.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATH_H_
#define MLI_MATH_H_


#define MLI_PI 3.14159265358979323846
#define MLI_2PI 6.28318530717958623199
#define MLI_2_OVER_SQRT3 1.1547005383792517
#define MLI_SQRT3_OVER_2 0.8660254037844386
#define MLI_EPSILON 1e-9
#define MLI_NAN 0. / 0.
#define MLI_IS_NAN(a) ((a) != (a))
#define MLI_MIN2(a, b) (((a) < (b)) ? (a) : (b))
#define MLI_MAX2(a, b) (((a) > (b)) ? (a) : (b))
#define MLI_ROUND(num) (num - floor(num) > 0.5) ? ceil(num) : floor(num)
#define MLI_NEAR_INT(x) ((x) > 0 ? (int64_t)((x) + 0.5) : (int64_t)((x)-0.5))

#define MLI_MIN3(a, b, c)                                                      \
        ((((a) < (b)) && ((a) < (c))) ? (a) : (((b) < (c)) ? (b) : (c)))

#define MLI_MAX3(a, b, c)                                                      \
        ((((a) > (b)) && ((a) > (c))) ? (a) : (((b) > (c)) ? (b) : (c)))

#define MLI_ARRAY_SET(arr, val, num)                                           \
        do {                                                                   \
                uint64_t i;                                                    \
                for (i = 0; i < num; i++) {                                    \
                        arr[i] = val;                                          \
                }                                                              \
        } while (0)

#define MLI_UPPER_COMPARE(points, num_points, point_arg, return_idx)           \
        do {                                                                   \
                uint64_t first, last, middle;                                  \
                first = 0u;                                                    \
                last = num_points - 1u;                                        \
                middle = (last - first) / 2;                                   \
                if (num_points == 0) {                                         \
                        return_idx = 0;                                        \
                } else {                                                       \
                        if (point_arg >= points[num_points - 1u]) {            \
                                return_idx = num_points;                       \
                        } else {                                               \
                                while (first < last) {                         \
                                        if (points[middle] > point_arg) {      \
                                                last = middle;                 \
                                        } else {                               \
                                                first = middle + 1u;           \
                                        }                                      \
                                        middle = first + (last - first) / 2;   \
                                }                                              \
                                return_idx = last;                             \
                        }                                                      \
                }                                                              \
        } while (0)

#define MLI_NCPY(src, dst, num)                                                \
        do {                                                                   \
                uint64_t i;                                                    \
                for (i = 0; i < num; i++) {                                    \
                        dst[i] = src[i];                                       \
                }                                                              \
        } while (0)

double mli_std(
        const double vals[],
        const uint64_t size,
        const double vals_mean);
double mli_mean(const double vals[], const uint64_t size);
void mli_linspace(
        const double start,
        const double stop,
        double *points,
        const uint64_t num_points);
void mli_histogram(
        const double *bin_edges,
        const uint64_t num_bin_edges,
        uint64_t *underflow_bin,
        uint64_t *bins,
        uint64_t *overflow_bin,
        const double point);
uint64_t mli_upper_compare_double(
        const double *points,
        const uint64_t num_points,
        const double point_arg);
double mli_square(const double a);
double mli_hypot(const double a, const double b);
double mli_deg2rad(const double angle_in_deg);
double mli_rad2deg(const double angle_in_rad);
double mli_bin_center_in_linear_space(
        const double start,
        const double stop,
        const uint64_t num_bins,
        const uint64_t bin);
double mli_linear_interpolate_1d(
        const double weight,
        const double start,
        const double end);
double mli_linear_interpolate_2d(
        const double xarg,
        const double x0,
        const double y0,
        const double x1,
        const double y1);
#endif



/* mli_cstr.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CSTR_H_
#define MLI_CSTR_H_

int mli_cstr_ends_with(const char *str, const char *sufix);
int mli_cstr_starts_with(const char *str, const char *prefix);
int mli_cstr_has_prefix_suffix(
        const char *str,
        const char *prefix,
        const char *sufix);

int mli_cstr_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length);
int mli_cstr_is_CRLF(const char *s);
int mli_cstr_is_CR(const char *s);
int mli_cstr_assert_only_NUL_LF_TAB_controls(const char *str);
int mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(
        const char *str,
        const int dbg);

uint64_t mli_cstr_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan);

int mli_cstr_lines_fprint(
        FILE *f,
        const char *str,
        const uint64_t line,
        const uint64_t line_radius);
void mli_cstr_path_strip_this_dir(char *dst, const char *src);

void mli_cstr_path_basename_without_extension(const char *filename, char *key);
void mli_cstr_strip_spaces(const char *in, char *out);

int mli_cstr_match_templeate(
        const char *s,
        const char *t,
        const char digit_wildcard);

#endif



/* mli_cstr_numbers.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CSTR_NUMBERS_H_
#define MLI_CSTR_NUMBERS_H_


int mli_cstr_nto_int64(
        int64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t length);
int mli_cstr_to_int64(int64_t *out, const char *s, const uint64_t base);

int mli_cstr_nto_uint64(
        uint64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t length);
int mli_cstr_to_uint64(uint64_t *out, const char *s, const uint64_t base);

int mli_cstr_nto_double(double *out, const char *s, const uint64_t length);
int mli_cstr_to_double(double *out, const char *s);

int mli_cstr_print_uint64(
        uint64_t u,
        char *s,
        const uint64_t max_num_chars,
        const uint64_t base,
        const uint64_t min_num_digits);

#endif



/* mliTar.h */
/**
 * Copyright (c) 2017 rxi
 * Copyright (c) 2019 Sebastian A. Mueller
 *                    Max-Planck-Institute for nuclear-physics, Heidelberg
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef MLITAR_H_
#define MLITAR_H_


#define MLI_TAR_NORMAL_FILE '0'
#define MLI_TAR_HARD_LINK '1'
#define MLI_TAR_SYMBOLIC_LINK '2'
#define MLI_TAR_CHARACTER_SPECIAL '3'
#define MLI_TAR_BLOCK_SPECIAL '4'
#define MLI_TAR_DIRECTORY '5'
#define MLI_TAR_FIFO '6'
#define MLI_TAR_NAME_LENGTH 100
#define MLI_TAR_OCTAL 8u
#define MLI_TAR_MAX_FILESIZE_OCTAL 8589934592lu /* 8^11 */

struct mliTarHeader {
        uint64_t mode;
        uint64_t owner;
        uint64_t size;
        uint64_t mtime;
        uint64_t type;
        char name[MLI_TAR_NAME_LENGTH];
        char linkname[MLI_TAR_NAME_LENGTH];
};

struct mliTarHeader mliTarHeader_init(void);
int mliTarHeader_set_directory(struct mliTarHeader *h, const char *name);
int mliTarHeader_set_normal_file(
        struct mliTarHeader *h,
        const char *name,
        const uint64_t size);

struct mliTar {
        FILE *stream;
        uint64_t pos;
        uint64_t remaining_data;
};

struct mliTar mliTar_init(void);
int mliTar_open(struct mliTar *tar, const char *filename, const char *mode);
int mliTar_finalize(struct mliTar *tar);
int mliTar_close(struct mliTar *tar);

int mliTar_read_header(struct mliTar *tar, struct mliTarHeader *h);
int mliTar_read_data(struct mliTar *tar, void *ptr, uint64_t size);

int mliTar_write_header(struct mliTar *tar, const struct mliTarHeader *h);
int mliTar_write_data(struct mliTar *tar, const void *data, uint64_t size);

/* internal */
int mliTar_uint64_to_field12_2001star_base256(uint64_t val, char *field);
int mliTar_field12_to_uint64_2001star_base256(const char *field, uint64_t *val);

#endif



/* mliDynArray.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIDYNARRAY_H_
#define MLIDYNARRAY_H_


#define MLIDYNARRAY_DEFINITON(LIB, NAME, PAYLOAD_TYPE)                         \
                                                                               \
        struct LIB##Dyn##NAME {                                                \
                uint64_t capacity;                                             \
                uint64_t size;                                                 \
                PAYLOAD_TYPE *array;                                           \
        };                                                                     \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void);                     \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *dh);                 \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *dh, const uint64_t size);               \
                                                                               \
        int LIB##Dyn##NAME##_malloc_set_size(                                  \
                struct LIB##Dyn##NAME *dh, const uint64_t size);               \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *dh, PAYLOAD_TYPE item);

#define MLIDYNARRAY_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)                    \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void)                      \
        {                                                                      \
                struct LIB##Dyn##NAME dh;                                      \
                dh.capacity = 0u;                                              \
                dh.size = 0u;                                                  \
                dh.array = NULL;                                               \
                return dh;                                                     \
        }                                                                      \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *dh)                  \
        {                                                                      \
                free(dh->array);                                               \
                (*dh) = LIB##Dyn##NAME##_init();                               \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *dh, const uint64_t size)                \
        {                                                                      \
                LIB##Dyn##NAME##_free(dh);                                     \
                dh->capacity = MLI_MAX2(2, size);                              \
                dh->size = 0;                                                  \
                chk_malloc(dh->array, PAYLOAD_TYPE, dh->capacity);             \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc_set_size(                                  \
                struct LIB##Dyn##NAME *dh, const uint64_t size)                \
        {                                                                      \
                chk(LIB##Dyn##NAME##_malloc(dh, size));                        \
                dh->size = size;                                               \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *dh, PAYLOAD_TYPE item)                  \
        {                                                                      \
                if (dh->size == dh->capacity) {                                \
                        dh->capacity = dh->capacity * 2;                       \
                        dh->array = (PAYLOAD_TYPE *)realloc(                   \
                                (void *)dh->array,                             \
                                dh->capacity * sizeof(PAYLOAD_TYPE));          \
                        chk_mem(dh->array);                                    \
                }                                                              \
                                                                               \
                dh->array[dh->size] = item;                                    \
                dh->size += 1;                                                 \
                                                                               \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }

#endif



/* mliDynFloat.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIDYNARRAY_FLOAT_H_
#define MLIDYNARRAY_FLOAT_H_

MLIDYNARRAY_DEFINITON(mli, Float, float)
#endif



/* mli_corsika_version.h */
/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CORSIKA_VERSION_H_
#define MLI_CORSIKA_VERSION_H_

#define MLI_CORSIKA_VERSION_MAYOR 0
#define MLI_CORSIKA_VERSION_MINOR 2
#define MLI_CORSIKA_VERSION_PATCH 0

#endif



/* mli_corsika_utils.h */
/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_UTILS_H_
#define MLI_CORSIKA_UTILS_H_

float mli_chars_to_float(const char *four_char_word);

#define MLI_CORSIKA_HEADER_SIZE_BYTES (sizeof(float) * 273)
#define MLI_CORSIKA_BUNCH_SIZE_BYTES (sizeof(float) * 8)

#define MLI_CORSIKA_RUNH_RUN_NUMBER 1
#define MLI_CORSIKA_RUNH_SLOPE_OF_ENERGY_SPECTRUM 15
#define MLI_CORSIKA_RUNH_ENERGY_RANGE_START 16
#define MLI_CORSIKA_RUNH_ENERGY_RANGE_STOP 17
#define MLI_CORSIKA_RUNH_NUM_OBSERVATION_LEVELS 4

#define MLI_CORSIKA_EVTH_EVENT_NUMBER 1
#define MLI_CORSIKA_EVTH_RUN_NUMBER 43
#define MLI_CORSIKA_EVTH_PARTICLE_ID 2
#define MLI_CORSIKA_EVTH_ENERGY_GEV 3
#define MLI_CORSIKA_EVTH_ZENITH_RAD 10
#define MLI_CORSIKA_EVTH_AZIMUTH_RAD 11
#define MLI_CORSIKA_EVTH_FIRST_INTERACTION_HEIGHT_CM 6

#endif



/* mli_corsika_EventTape.h */
/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_EVENTTAPE_H_
#define MLI_CORSIKA_EVENTTAPE_H_



#define MLI_CORSIKA_EVENTTAPE_VERSION_MAYOR 2
#define MLI_CORSIKA_EVENTTAPE_VERSION_MINOR 0
#define MLI_CORSIKA_EVENTTAPE_VERSION_PATCH 0

struct mliEventTapeWriter {
        struct mliTar tar;
        int run_number;
        int event_number;
        int cherenkov_bunch_block_number;
        struct mliDynFloat buffer;
};
struct mliEventTapeWriter mliEventTapeWriter_init(void);
int mliEventTapeWriter_open(
        struct mliEventTapeWriter *tio,
        const char *path,
        const uint64_t num_bunches_buffer);
int mliEventTapeWriter_close(struct mliEventTapeWriter *tio);
int mliEventTapeWriter_write_runh(
        struct mliEventTapeWriter *tio,
        const float *runh);
int mliEventTapeWriter_write_evth(
        struct mliEventTapeWriter *tio,
        const float *evth);
int mliEventTapeWriter_write_cherenkov_bunch(
        struct mliEventTapeWriter *tio,
        const float *bunch);
int mliEventTapeWriter_flush_cherenkov_bunch_block(
        struct mliEventTapeWriter *tio);

struct mliEventTapeReader {
        uint64_t run_number;

        /* Current event-number */
        uint64_t event_number;

        /* Current cherenkov-block-number inside the current event */
        uint64_t cherenkov_bunch_block_number;

        /* Current bunch-number inside the current cherenkov-block */
        uint64_t block_at;
        uint64_t block_size;
        int has_still_bunches_in_event;

        /* Underlying tape-archive */
        struct mliTar tar;

        /* Next file's tar-header in the underlying tape-archive */
        int has_tarh;
        struct mliTarHeader tarh;
};
struct mliEventTapeReader mliEventTapeReader_init(void);
int mliEventTapeReader_open(struct mliEventTapeReader *tio, const char *path);
int mliEventTapeReader_close(struct mliEventTapeReader *tio);
int mliEventTapeReader_read_runh(struct mliEventTapeReader *tio, float *runh);
int mliEventTapeReader_read_evth(struct mliEventTapeReader *tio, float *evth);
int mliEventTapeReader_read_cherenkov_bunch(
        struct mliEventTapeReader *tio,
        float *bunch);
int mliEventTapeReader_tarh_is_valid_cherenkov_block(
        const struct mliEventTapeReader *tio);
int mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
        const struct mliEventTapeReader *tio);
int mliEventTapeReader_read_readme_until_runh(struct mliEventTapeReader *tio);

#endif



