Welcome to Marrinator

Marrinator is a C++ library for AVR processors, including Arduino. It's also a build system that runs on Macs using Xcode.

================

Why?
----------
Marrinator provides a very rudimentary operating system which includes interrupt servicing and event processing. It relies heavily on C++ templates and therefore many things that would require passing runtime parameters can be done with template parameters. This means they are resolved at compile time and avoid any runtime overhead at all. Furthermore, templated bit ports allow you to say:

    OutputBit<B, 5> led;

    led = true;
    led = !led;

When fully optimized these reduce to a single instruction. So there's no overhead incurred for more elegant and readable code.

C++ on AVR? Seriously?
-----------------------------------
I've seen lots of push back about using C++ on AVR. Much of this is old prejudice about C++ being somehow inherently less efficient than C. But advances in compiler design mostly make these issues go away. And in some ways it's possible C++ more efficient than C. This is true both because of templates as I mentioned before and the inherent use of inlining in C++ classes. There is a tiny bit of overhead with C++ for global constructors and the like. And using virtual functions has its own expenses, both in terms of code and data used. So advanced features like this should be used sparingly. But marrinator makes use of virtual functions in event handler and that makes them much easier to write in a maintainable way. 

The build system
------------------------
Another goal of marrinator was to make it possible to build AVR apps and program the chips without using Makefiles. Xcode makes it possible to build projects for other processors. But this usually involves an External Build Target and that typically means Makefiles. But after quite a bit of work I discovered that it was possible to override build rules, add scripts and override some of the built-in functionality to arrive at a system that is fully integrated with the IDE. To add a file you simply drop it into the project navigator and hit build. You source is even indexed so you can use code completion and quickly find where variables are defined.

All the build details are in the xcconfig files. Mostly you just need to look at the first few lines of Shared.xcconfig. That's where you set the MCU, F_CPU and programmer. Right now it only supports AVR Dragon and STK500. But I'm working on making it work with the Arduino bootloader. It assumes AVR Crosspack for the build tools and that's the only thing it's tested with. I highly recommend this package. It's a simple download and install. Shared.xcconfig is already pointing at its standard install location.

Debugging
----------------
Marrinator provides 3 build types: Debug, Report and Release. Debug and Release are just like in any build system. Report builds using the optimizations of Release with the DEBUG flag turned on. So ASSERT() works, and there are NOTE() and WARNING macros to allow rudimentary debugging. There is even a built in error reporter. This can be subclassed to output to any device, like the serial port or an attached LCD display. But it also has a built-in BlinkErrorReporter, this blinks the codes you supply to NOTE() and WARNING() so you can do debugging with nothing more than the LED that is attached to most Arduino boards.

How to use it
--------------------
Marrinator is a work in progress. If you go to the package/ folder you'll find everything you need to get started. There is a marrinator.xcodeproj which builds the library and a sample application file. This file simply blinks the Arduino light, but does it in 4 ways: with a simple busy loop (timed according to F_CPU), a timer which ejects events into the system, and two styles of real-time clock ticks.

I'm currently looking at ways to package up the library so it lives in a common place. Until then, just take the sample app and change it as you like.

Have fun!

