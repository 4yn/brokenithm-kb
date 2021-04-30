/*
 * Authored by Alex Hultman, 2018-2019.
 * Intellectual property of third-party.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This standalone module implements deflate / inflate streams */

#ifndef UWS_PERMESSAGEDEFLATE_H
#define UWS_PERMESSAGEDEFLATE_H

#ifndef UWS_NO_ZLIB
#include <zlib.h>
#endif

#include <string>

namespace uWS {

/* Do not compile this module if we don't want it */
#ifdef UWS_NO_ZLIB
struct ZlibContext {};
struct InflationStream {
    std::string_view inflate(ZlibContext *zlibContext, std::string_view compressed, size_t maxPayloadLength) {
        return compressed;
    }
};
struct DeflationStream {
    std::string_view deflate(ZlibContext *zlibContext, std::string_view raw, bool reset) {
        return raw;
    }
};
#else

#define LARGE_BUFFER_SIZE 1024 * 16 // todo: fix this

struct ZlibContext {
    /* Any returned data is valid until next same-class call.
     * We need to have two classes to allow inflation followed
     * by many deflations without modifying the inflation */
    std::string dynamicDeflationBuffer;
    std::string dynamicInflationBuffer;
    char *deflationBuffer;
    char *inflationBuffer;

    ZlibContext() {
        deflationBuffer = (char *) malloc(LARGE_BUFFER_SIZE);
        inflationBuffer = (char *) malloc(LARGE_BUFFER_SIZE);
    }

    ~ZlibContext() {
        free(deflationBuffer);
        free(inflationBuffer);
    }
};

struct DeflationStream {
    z_stream deflationStream = {};

    DeflationStream() {
        deflateInit2(&deflationStream, 1, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
    }

    /* Deflate and optionally reset */
    std::string_view deflate(ZlibContext *zlibContext, std::string_view raw, bool reset) {
        /* Odd place to clear this one, fix */
        zlibContext->dynamicDeflationBuffer.clear();

        deflationStream.next_in = (Bytef *) raw.data();
        deflationStream.avail_in = (unsigned int) raw.length();

        /* This buffer size has to be at least 6 bytes for Z_SYNC_FLUSH to work */
        const int DEFLATE_OUTPUT_CHUNK = LARGE_BUFFER_SIZE;

        int err;
        do {
            deflationStream.next_out = (Bytef *) zlibContext->deflationBuffer;
            deflationStream.avail_out = DEFLATE_OUTPUT_CHUNK;

            err = ::deflate(&deflationStream, Z_SYNC_FLUSH);
            if (Z_OK == err && deflationStream.avail_out == 0) {
                zlibContext->dynamicDeflationBuffer.append(zlibContext->deflationBuffer, DEFLATE_OUTPUT_CHUNK - deflationStream.avail_out);
                continue;
            } else {
                break;
            }
        } while (true);

        /* This must not change avail_out */
        if (reset) {
            deflateReset(&deflationStream);
        }

        if (zlibContext->dynamicDeflationBuffer.length()) {
            zlibContext->dynamicDeflationBuffer.append(zlibContext->deflationBuffer, DEFLATE_OUTPUT_CHUNK - deflationStream.avail_out);

            return {(char *) zlibContext->dynamicDeflationBuffer.data(), zlibContext->dynamicDeflationBuffer.length() - 4};
        }

        return {
            zlibContext->deflationBuffer,
            DEFLATE_OUTPUT_CHUNK - deflationStream.avail_out - 4
        };
    }

    ~DeflationStream() {
        deflateEnd(&deflationStream);
    }
};

struct InflationStream {
    z_stream inflationStream = {};

    InflationStream() {
        inflateInit2(&inflationStream, -15);
    }

    ~InflationStream() {
        inflateEnd(&inflationStream);
    }

    std::string_view inflate(ZlibContext *zlibContext, std::string_view compressed, size_t maxPayloadLength) {

        /* We clear this one here, could be done better */
        zlibContext->dynamicInflationBuffer.clear();

        inflationStream.next_in = (Bytef *) compressed.data();
        inflationStream.avail_in = (unsigned int) compressed.length();

        int err;
        do {
            inflationStream.next_out = (Bytef *) zlibContext->inflationBuffer;
            inflationStream.avail_out = LARGE_BUFFER_SIZE;

            err = ::inflate(&inflationStream, Z_SYNC_FLUSH);
            if (err == Z_OK && inflationStream.avail_out) {
                break;
            }

            zlibContext->dynamicInflationBuffer.append(zlibContext->inflationBuffer, LARGE_BUFFER_SIZE - inflationStream.avail_out);


        } while (inflationStream.avail_out == 0 && zlibContext->dynamicInflationBuffer.length() <= maxPayloadLength);

        inflateReset(&inflationStream);

        if ((err != Z_BUF_ERROR && err != Z_OK) || zlibContext->dynamicInflationBuffer.length() > maxPayloadLength) {
            return {nullptr, 0};
        }

        if (zlibContext->dynamicInflationBuffer.length()) {
            zlibContext->dynamicInflationBuffer.append(zlibContext->inflationBuffer, LARGE_BUFFER_SIZE - inflationStream.avail_out);

            /* Let's be strict about the max size */
            if (zlibContext->dynamicInflationBuffer.length() > maxPayloadLength) {
                return {nullptr, 0};
            }

            return {zlibContext->dynamicInflationBuffer.data(), zlibContext->dynamicInflationBuffer.length()};
        }

        /* Let's be strict about the max size */
        if ((LARGE_BUFFER_SIZE - inflationStream.avail_out) > maxPayloadLength) {
            return {nullptr, 0};
        }

        return {zlibContext->inflationBuffer, LARGE_BUFFER_SIZE - inflationStream.avail_out};
    }

};

#endif

}

#endif // UWS_PERMESSAGEDEFLATE_H
