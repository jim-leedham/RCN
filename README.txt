Summary
-------

This is a stabilised fork of the RECON (RCN) engine intended for review purposes. 
This fork deliberately contains very little game code; only a short demo designed 
to showcase the most commonly used features of the engine.

Source code may be viewed:
- In a web browser (at https://bitbucket.org/JimLeedham/rcn001-evaluation)
- In Visual Studio (the provided solution file is compatible with Visual Studio 2012)
- In another text editor 

An executable, RCN001.exe, is also provided at the repository root.

RECON
-----

The RECON engine offers the following features:
- 3D Utilities including Vector and Matrix manipulation
- 2D Screen controller
- Audio controller with support of XAudio2, OpenAL and OpenSL and configurable via XML
- Input controller with queryable key states
- Job queues for asynchronous  asset loading with custom thread construction
- Platform-agnostic rendering utilities for:
-- Lighting
-- Materials
-- Models
-- Textures
-- Fonts
- Memory Utilities including allocation guards and leak detection
- Terrain generator with built-in Simplex noise functions
- XML Utilities

Note that while all aspects of the engine are designed to be compatible with any platform
or graphics API, many features still only contain implementations for DirectX on Windows.

Scheduled Features
------------------

- Exception handling
- Animation controller
- Language controller


