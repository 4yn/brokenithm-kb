# Repackaged uWebsockets library

Source was obtained from [uSockets v0.7.1](https://github.com/uNetworking/uSockets/tree/v0.7.1) and [uWebSockets v19.2.0](https://github.com/uNetworking/uWebSockets/tree/v19.2.0) and repackaged with build system configuration.

# Original uWebsockets README.md

<div align="center">
<img src="https://raw.githubusercontent.com/uNetworking/uWebSockets/master/misc/logo.svg" height="180" /><br>
<i>Simple, secure</i><sup><a href="https://github.com/uNetworking/uWebSockets/tree/master/fuzzing#fuzz-testing-of-various-parsers-and-mocked-examples">[1]</a></sup><i> & standards compliant</i><sup><a href="https://unetworking.github.io/uWebSockets.js/report.pdf">[2]</a></sup><i> web server for the most demanding</i><sup><a href="https://github.com/uNetworking/uWebSockets/tree/master/benchmarks#benchmark-driven-development">[3]</a></sup><i> of applications.</i> <a href="https://github.com/uNetworking/uWebSockets/blob/master/misc/READMORE.md">Read more...</a>
<br><br>

<a href="https://github.com/uNetworking/uWebSockets/releases"><img src="https://img.shields.io/github/v/release/uNetworking/uWebSockets"></a> <a href="https://lgtm.com/projects/g/uNetworking/uWebSockets/context:cpp"><img alt="Language grade: C/C++" src="https://img.shields.io/lgtm/grade/cpp/g/uNetworking/uWebSockets.svg?logo=lgtm&logoWidth=18"/></a> <a href="https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:uwebsockets"><img src="https://oss-fuzz-build-logs.storage.googleapis.com/badges/uwebsockets.svg" /></a> <img src="https://img.shields.io/badge/downloads-60%20million-pink" />

</div>
<br><br>

### :closed_lock_with_key: Optimized security

Being meticulously optimized for speed and memory footprint, µWebSockets is fast enough to do encrypted TLS 1.3 messaging quicker than most alternative servers can do even unencrypted, cleartext messaging<sup><a href="https://github.com/uNetworking/uWebSockets/tree/master/benchmarks#benchmark-driven-development">[3]</a></sup>.

Furthermore, we partake in Google's OSS-Fuzz with a ~95% daily fuzzing coverage with no sanitizer issues. LGTM scores us flawless A+ from having zero CodeQL alerts and we compile with pedantic warning levels.

### :arrow_forward: Rapid scripting

µWebSockets is written entirely in C & C++ but has a seamless integration for Node.js backends. This allows for rapid scripting of powerful apps, using widespread competence. See <a href="https://github.com/uNetworking/uWebSockets.js">µWebSockets.js</a>.

### :crossed_swords: Battle proven

We've been fully standards compliant with a perfect Autobahn|Testsuite score since 2016<sup><a href="https://unetworking.github.io/uWebSockets.js/report.pdf">[2]</a></sup>. Companies with everything to lose rely on µWebSockets on a daily basis - we power the trading APIs of [Bitfinex.com](https://bitfinex.com) & [Kraken.com](https://www.kraken.com/), handling volumes of multiple billions of USD every day. Other companies include [Trello](https://trello.com), where µWebSockets is serving their 50 million users with real-time board updates.

### :battery: Batteries included

Designed around a convenient URL router with wildcard & parameter support - paired with efficient pub/sub features inspired by MQTT. µWebSockets should be the obvious, complete starting point for any real-time web project with high demands.

Start building your Http & WebSocket apps in a swift; <a href="https://github.com/uNetworking/uWebSockets/blob/master/misc/READMORE.md">read the user manual</a> and <a href="https://github.com/uNetworking/uWebSockets/tree/master/examples">see examples</a>. You can browse our <a href="https://unetworking.github.io/uWebSockets.js/generated/">TypeDoc</a> for a quick overview.

```c++
uWS::SSLApp({

    /* There are tons of SSL options, see uSockets */
    .cert_file_name = "cert.pem",
    .key_file_name = "key.pem"

}).get("/hello", [](auto *res, auto *req) {

    /* You can efficiently stream huge files too */
    res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");

}).ws<UserData>("/*", {

    /* Just a few of the available handlers */
    .open = [](auto *ws) {
        /* MQTT syntax */
        ws->subscribe("sensors/+/house");
    },
    .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
        ws->send(message, opCode);
    }

}).listen(9001, [](auto *listenSocket) {

    if (listenSocket) {
        std::cout << "Listening on port " << 9001 << std::endl;
    }

}).run();
```

### :briefcase: Commercially supported

<a href="https://github.com/uNetworking">uNetworking AB</a> is a Swedish consulting & contracting company dealing with anything related to µWebSockets; development, support and customer success.

Don't hesitate <a href="mailto:alexhultman@gmail.com">sending a mail</a> if you're building something large, in need of advice or having other business inquiries in mind. We'll figure out what's best for both parties and make sure you're not stepping into one of the many common pitfalls.

Special thanks to BitMEX, Bitfinex, Google, Coinbase, Bitwyre and deepstreamHub for allowing the project itself to thrive on GitHub since 2016 - this project would not be possible without these beautiful companies.

<img src="https://github.com/uNetworking/uWebSockets/raw/master/misc/2018.png" />

### :wrench: Customizable architecture

µWebSockets builds on <a href="https://github.com/uNetworking/uSockets">µSockets</a>, a foundation library implementing eventing, networking and cryptography in three different layers. Every layer has multiple implementations and you control the compiled composition with flags.

In a nutshell:

- `WITH_WOLFSSL=1 WITH_LIBUV=1 make examples` builds examples utilizing WolfSSL and libuv
- `WITH_OPENSSL=1 make examples` builds examples utilizing OpenSSL and the native kernel

See µSockets for an up-to-date list of flags and a more detailed explanation.

### :handshake: Permissively licensed

Intellectual property, all rights reserved.

Where such explicit notice is given, source code is licensed Apache License 2.0 which is a permissive OSI-approved license with very few limitations. Modified "forks" should be of nothing but licensed source code, and be made available under another product name. If you're uncertain about any of this, please ask before assuming.

# Original uSockets README.md

## µSockets - miniscule networking & eventing

This is the cross-platform async networking and eventing foundation library used by [µWebSockets](https://github.com/uNetworking/uWebSockets).

It runs on Linux, macOS, FreeBSD and Windows. Most is C11 while some optional parts are C++17.

<a href="https://github.com/uNetworking/uSockets/releases"><img src="https://img.shields.io/github/v/release/uNetworking/uSockets"></a> <a href="https://lgtm.com/projects/g/uNetworking/uSockets/context:cpp"><img alt="Language grade: C/C++" src="https://img.shields.io/lgtm/grade/cpp/g/uNetworking/uSockets.svg?logo=lgtm&logoWidth=18"/></a>

### Key aspects

- Built-in (optionally available) TLS support exposed with identical interface as for TCP.
- Acknowledges and integrates with any event-loop via a layered hierarchical design of plugins.
- Extremely pedantic about user space memory footprint and designed to perform as good as can be.
- Designed from scratch to map well to user space TCP stacks or other experimental platforms.
- Low resolution timer system ideal for performant tracking of networking timeouts.
- Minimal yet truly cross-platform, will not emit a billion different platform specific error codes.
- Fully opaque library, inclusion will not completely pollute your global namespace.
- Continuously fuzzed by Google's [OSS-Fuzz](https://github.com/google/oss-fuzz) with 95% code coverage (asan, ubsan and msan).

### Extensible

Designed in layers of abstraction where any one layer depends only on the previous one. Write plugins and swap things out with compiler flags as you see fit.

![](misc/layout.png)

### Compilation

Build example binaries using `make examples`. The static library itself builds with `make`. It is also possible to simply include the `src` folder in your project as it is standard C11. Defining LIBUS_NO_SSL (-DLIBUS_NO_SSL) will disable OpenSSL 1.1+ support/dependency (not needed if building with shipped Makefile). Build with environment variables set as shown below to configure for specific needs.

##### Available plugins

- Build using `WITH_LIBUV=1 make [examples]` to use libuv as event-loop.
- Build using `WITH_GCD=1 make [examples]` to use Grand Central Dispatch/CoreFoundation as event-loop (slower).
- Build using `WITH_OPENSSL=1 make [examples]` to enable and link OpenSSL 1.1+ support (or BoringSSL).
- Build using `WITH_WOLFSSL=1 make [examples]` to enable and link WolfSSL 2.4.0+ support for embedded use.

The default event-loop is native epoll on Linux, native kqueue on macOS and finally libuv on Windows.

##### A word on performance

This library is opaque; there are function calls for everything - even simple things like accessing the "user data" of a socket. In other words, static linking and link-time-optimizations mean **everything** for performance. I've benchmarked dynamic linking vs. static, link-time optimization and found roughly a 60% performance difference.

The kernel you run makes a huge difference. Linux wins, hands down. Mitigations off, or a modern hardware-mitigated CPU makes huge differences and distros like Clear Linux have shown significant speedups compared to more "vanilla" kernels.
