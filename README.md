# C2server

## History of dontnet

The history of the Microsoft .NET platform is divided into three major eras: the original .NET Framework (Windows-only), .NET Core (cross-platform), and the modern unified .NET (starting with .NET 5). 
1. The .NET Framework Era (2002–2022) 
This was the original implementation designed specifically for Windows. It remains a key part of the Windows OS, with 4.8.1 being the final major version. 

Version 	Release Date	Key Features & Impact
- 1.0	Feb 2002	The debut version featuring the Common Language Runtime (CLR).
- 1.1	Apr 2003	Added mobile ASP.NET controls and IPv6 support.
- 2.0	Nov 2005	Introduced generics, partial classes, and nullable types.
- 3.0	Nov 2006	Introduced WPF (UI), WCF (communication), and WF (workflow).
- 3.5	Nov 2007	Added LINQ (Language Integrated Query) and AJAX support.
- 4.0	Apr 2010	Added the Task Parallel Library (TPL) for parallel programming.
- 4.5	Aug 2012	Introduced the async/await pattern for simplified asynchronous code.
- 4.6.x	2015–2016	Added a new "RyuJIT" compiler and better security (TLS 1.1/1.2).
- 4.7.x	2017–2018	Focused on high-DPI support and .NET Standard 2.0 compatibility.
- 4.8 / 4.8.1	2019 / 2022	Final major releases; 4.8.1 added native ARM64 support.
2. The .NET Core Era (2016–2019)
Microsoft rebuilt the platform from scratch to be open-source and cross-platform (running on Windows, macOS, and Linux). 

- .NET Core 1.0 & 1.1 (2016–2017): First modular, cross-platform releases.
- .NET Core 2.0 & 2.1 (2017–2018): Significant expansion of APIs to improve migration from .NET Framework.
- .NET Core 3.0 & 3.1 (2019): Added support for Windows desktop apps (WPF/WinForms) to Core. 

3. The Unified .NET Era (2020–Present)
Starting with .NET 5, Microsoft dropped "Core" and unified the ecosystem into a single platform for all app types. These versions follow an annual release cycle every November. 
 +2
- .NET 5 (2020): The first unified release; skipped version 4 to avoid confusion with .NET Framework 4.x.
- .NET 6 (2021): LTS (Long-Term Support) version; introduced .NET MAUI for cross-platform UI.
- .NET 7 (2022): Standard Term Support (STS) with focus on performance and cloud-native features.
- .NET 8 (2023): LTS version; current mainstream release with major AI and cloud-native enhancements.
- .NET 9 (2024): Current STS version with runtime optimizations.
- .NET 10 (Nov 2025): Upcoming LTS release. 

## Dotnet loader 
### why dotnet codes for injection
In a red team or offensive security context, running managed code (C#/.NET) from an unmanaged process (like explorer.exe, svchost.exe, or a custom C++ loader) is a powerful technique often referred to as "Bring Your Own Runtime."
Here are the primary benefits for an operator:
1. Evasion of Static Analysis
Standard C++ malware is often flagged based on suspicious Win32 API imports in the Header (e.g., CreateRemoteThread).
The Benefit: By hosting the CLR inside a legitimate process, you can load your malicious .NET assembly directly into memory. The file on disk might look like a benign C++ tool or even be a completely "fileless" payload delivered via a macro or script, leaving very little for traditional scanners to grab.
2. In-Memory Execution (Post-Exploitation)
Tools like Cobalt Strike use this via the execute-assembly command.
The Benefit: You can load a .NET DLL (like Rubeus, SharpHound, or Certify) into the memory of a remote process without it ever touching the disk. This bypasses many legacy Antivirus (AV) solutions that only scan files as they are created or modified.
3. Access to Powerful APIs with Less "Noise"
Writing complex networking or crypto code in C++ is tedious and error-prone.
The Benefit: Managed code gives you access to the massive .NET Base Class Library (BCL). You can perform complex tasks—like interacting with Active Directory, Web Requests, or WMI—with just a few lines of C#. The CLR handles the "noisy" low-level plumbing that EDRs (Endpoint Detection and Response) often monitor.
4. Bypassing EDR Hooks
Many EDRs monitor unmanaged code by "hooking" common Win32 APIs in ntdll.dll or kernel32.dll.
The Benefit: While modern EDRs are getting better at monitoring .NET via ETW (Event Tracing for Windows), older or poorly configured sensors focus heavily on the unmanaged side. Running inside the CLR adds a layer of abstraction that can sometimes slip past these hooks.
5. Blending In
The CLR is a "trusted" entity.
The Benefit: Many enterprise environments have legitimate .NET applications running everywhere. A process loading the CLR (using mscoree.dll) is a common, everyday event. To a basic monitoring tool, your malicious activity might look like a standard background task or a corporate management script.


### Methods to bypass AMSI
The Antimalware Scan Interface (AMSI) is essentially a "bridge" between an application and the installed antivirus (AV) software. Since .NET Framework 4.8, Microsoft integrated AMSI directly into the Common Language Runtime (CLR) to catch malicious code that never touches the disk. 

Here is how AMSI specifically targets .NET assemblies in memory:
1. The "Border Crossing" Hook
AMSI is strategically placed at the exact moment an assembly is loaded into memory.
Trigger Point: Whenever a program calls a method like Assembly.Load(byte[]), the CLR pauses execution.
The Hand-off: Before the assembly is allowed to run, the CLR takes the raw byte array and passes it to the AmsiScanBuffer function inside amsi.dll.
Verification: The AV engine (like Microsoft Defender) then scans these bytes for known signatures or malicious patterns. If it's flagged as malicious (result code 32768 or higher), the CLR throws an exception and prevents the code from executing. 

2. Defeating Obfuscation
The biggest strength of AMSI is its ability to see "unpacked" code. 
Script Unrolling: For scripted environments like PowerShell, AMSI scans the code after all layers of encryption, Base64 encoding, or XOR loops have been unwound by the interpreter.
JIT Inspection: It can evaluate the Common Intermediate Language (CIL) before it is turned into machine code, allowing it to catch malicious intent that static file scanners might miss. 

3. Session Correlation
AMSI uses a "session" concept to track behavior over time. 

Piece-by-Piece: If an attacker tries to load a malicious payload in small, seemingly harmless fragments, AMSI can correlate these fragments within a single session to see the full picture.
Contextual Scanning: It doesn't just look at the code; it also sees the "Content Name" (like a URL or unique script ID) to determine the source and reliability of the data. 

Common Red Team Bypasses
Because AMSI lives in the same "user-mode" memory as the application it's protecting, it is vulnerable to several bypasses: 
Memory Patching: Attackers find the AmsiScanBuffer function in memory and overwrite the first few bytes with a "return" instruction (e.g., mov eax, 0; ret) to force it to always report that the code is "Clean".
Hardware Breakpoints: A stealthier method uses CPU hardware registers to "trip" the process right before a scan occurs, allowing the attacker to skip the check without modifying the actual code.
Reflection: Using .NET's own reflection capabilities to find and disable the internal AMSI initialization flags within the CLR.

#### Elaborating

Since AMSI runs inside your process, your code (the "guest") has the same permissions as the defender (the "security guard"). Here is how those three common bypasses work at a technical level:
1. Memory Patching (The "Lobotomy")
This is the most common method. When a process starts, amsi.dll is loaded into its memory space.
The Logic: Every scan goes through a specific function called AmsiScanBuffer. This function normally does its job and returns a "Result" (Clean or Malicious).
The Hack: An attacker uses Win32 APIs like WriteProcessMemory to find the start of AmsiScanBuffer and overwrite the very first instructions with a "Return" command.
The Result: When the CLR tries to send an assembly to be scanned, it calls the function, but the function immediately quits and says "Everything is fine" before the scanning logic ever runs.
2. Hardware Breakpoints (The "Magic Trick")
Modern EDRs (Endpoint Detection and Response) now look for "patched" memory. If they see AmsiScanBuffer has been modified, they trigger an alert. Hardware breakpoints bypass this by not changing a single byte of code.
The Logic: CPUs have special registers (DR0-DR7) used for debugging. You can tell the CPU: "If the instruction pointer ever hits this specific memory address (AmsiScanBuffer), pause execution and call my custom function."
The Hack: The attacker sets a hardware breakpoint on the AMSI function. When the function is called, the CPU "trips," the attacker’s code takes control, changes the function's arguments to say "this is clean," and then tells the CPU to skip ahead.
The Result: The original amsi.dll remains untouched on a byte-level, making it invisible to "integrity checks" that look for memory patching.
3. Reflection (The "Internal Switch")
The CLR has internal variables that track whether AMSI is initialized and active. Since C# has Reflection, it can "look inside itself" and modify its own private settings.
The Logic: Inside the .NET Framework's System.Management.Automation or the CLR's internal classes, there is often a boolean field like amsiInitFailed or amsiContext.
The Hack: Using Reflection, an attacker can find these private, non-public fields in memory and manually flip the value from false to true (essentially lying to the system that AMSI failed to start).
The Result: The CLR thinks, "Oh, AMSI didn't load correctly, I'll just skip the security checks so the app doesn't crash." It effectively disables the guard by flipping a light switch.


## Hosting CLR
`https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/clr-hosting-interfaces-added-in-the-net-framework-4-and-4-5`

### CLR Hosting Interfaces Added in the .NET Framework 4 and 4.5


This section describes interfaces that unmanaged hosts can use to integrate the common language runtime (CLR) in the .NET Framework 4, .NET Framework 4.5, and later versions into their applications. These interfaces provide methods for a host to configure and load the runtime into a process.

Starting with the .NET Framework 4, all hosting interfaces have the following characteristics:

They use lifetime management (AddRef and Release), encapsulation (implicit context) and QueryInterface from COM.

They do not use COM types such as BSTR, SAFEARRAY, or VARIANT.

There are no apartment models, aggregation, or registry activation that use the CoCreateInstance function.

In This Section
- **ICLRAppDomainResourceMonitor Interface**:
  Provides methods that inspect an application domain's memory and CPU usage.

- **ICLRDomainManager Interface**:
  Enables the host to specify the application domain manager that will be used to initialize the default application domain, and to specify initialization properties.

- **ICLRGCManager2 Interface**:
  Provides the SetGCStartupLimitsEx method, which enables a host to set the size of the garbage collection segment and the maximum size of the garbage collection system's generation 0 to values greater than DWORD.

- **ICLRMetaHost Interface**:
Provides methods that return a specific version of the CLR, list all installed CLRs, list all in-process runtimes, return the activation interface, and discover the CLR version used to compile an assembly.

- **ICLRMetaHostPolicy Interface**:
Provides the GetRequestedRuntime method that provides a CLR interface based on policy criteria, managed assembly, version, and configuration file.

- **ICLRRuntimeInfo Interface**:
Provides methods that return information about a specific runtime, including version, directory, and load status.

- **ICLRStrongName Interface**:
Provides basic global static functions for signing assemblies with strong names. All the ICLRStrongName methods return standard COM HRESULTs.

- **ICLRStrongName2 Interface**:
Provides the ability to create strong names using the SHA-2 group of Secure Hash Algorithms (SHA-256, SHA-384, and SHA-512).

- **ICLRTask2 Interface**:
Provides all the functionality of the ICLRTask Interface; in addition, provides methods that allow thread aborts to be delayed on the current thread.




## references
https://learn.microsoft.com/en-us/dotnet/framework/unmanaged-api/hosting/clr-hosting-interfaces-added-in-the-net-framework-4-and-4-5
