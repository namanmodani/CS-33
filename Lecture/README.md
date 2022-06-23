# CS 33: Introduction to Computer Organization

Notes from class. Adapted from @JohnnyKong[https://github.com/JonnyKong] at Purdue, adjusted for CS 33.

# Integer representation
- Data types
    - char, short, int, long, float, double, pointer
    - Word size equals length of pointer datatype
- Bit-level operations
- Signed / unsigned conversion
- Byte ordering
    - Big Endian: Sun, PPC Mac, Internet
    - Little Endian: x86, ARM  
    <img src="Images/endian.png" width="50%">

# x86-64
- History
    - 8086 -> 386 -> Pentium 4E -> Core 2 -> Core i7
    - IA32 -> x86-64
    - CISC architecture
    - Case study: Core-i7 6700K Skylake
        - Shared L3 cache (LLC)
        - Hyper-Threading: Shared caches, buses & ALUs
        <img src="Images/corei7.png" width="70%">
- Registers  
    - Can reference low-order 4 bytes (backwards compatibility)
    - `%rsp` is stack top, others are general-purpose  
        <img src="Images/registers.png" width="50%">  
    - `%rip` is instruction pointer (not listed)  
    - A register for implicitly-set condition codes

# Machine-level Programming
- Addressing modes
    - Normal: `(R)` -> `Mem[Reg[R]]`
    - Displacement: `D(R)` -> `Mem[Reg[R] + D]`
    - Complete: `D(Rb,Ri,S)` -> `Mem[Reg[Rb] + S-Reg[Ri] + D]`
        - `(Rb,Ri)` -> `Mem[Reg[Rb] + Reg[Ri]]`
        - `D(Rb,Ri)` -> `Mem[Reg[Rb] + Reg[Ri] + D]`
        - `(Rb,Ri,S)` -> `Mem[Reg[Rb] + S-Reg[Ri]]`
- Some instructions
    - `movq Src, Dst`
        - Cannot do memory-memory transfer with a single instruction
        - Intel docs use `mov Dst, Src`
    - `leaq Src, Dst`
        - Src is address mode expression, set Dst to address denoted by expression
        - Similar to `p = &x[i]`
        - Used for arithmetics for form like `x + k - y`
        - Does not change condition codes
    - `addq/subq Src, Dst`
    - `imulq Src, Dst`
    - `salq/sarq/shrq Src, Dst`
    - `xorq/andq/orq Src, Dst`
    - `pushq src`
    - `popq dest`
    - `incr dest`
- Compiler, Assembler, Linker & Loader  
    1. Compiler
        - Translates C files (.c) into assembly files (.s)
    2. Assembler
        - Translates assembly files (.s) into object files (.o)
        - Missing linkage between compilation units
    3. Linker
        - Resolve references between object files
        - Combine with static libraries (malloc, printf, etc)
    4. Dynamic linked libraries
        - Linking occurs at runtime
        - Does not take too much disk space  
    <img src="Images/compilation.png" width="50%">  
- Controls
    - Jumping
        - `jmp`, `je`, `jne`, `js` ...
        - However, branches are very disruptive to instruction flow through pipelines
    - Conditional moves: `cmovle`
        - Do not require control transfer
        - Use `-fno-if-conversion` flag to supress implicit conversion
    - Switch statements
        - Jump table structure  
            <img src="Images/jump_table.png" width="80%">  
        - Make use of "fall through"
- Procedures
    - Passing control
        - Procedure call: `call label`
            - Push return address into stack
            - Jump to label
        - Procedure return: `ret`
            - Pop return address from stack
            - Jump to this address
        - Return address: Address of next instruction after the call statement
    - Passing data
        - First 6 arguments: `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`
        - Other arguments passed using stack
        - Return value: `%rax`
        - IA-32 pass all arguments in stack
        - Concept of stack frames:
            - Marked by `%rbp` (optional) and `%rsp`
            - No additional mechanism for recursion is needed  
            <img src="Images/stack_frame.png" width="60%">
        - Register saving conditions
            - Caller saved
                - `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`, `%rax`, `%r10`, `%r11`
            - Callee saved
                - `%rbx`, `%r12`, `%r13`, `%r14`, `%rbp`
                - `%rsp` is also a special form of callee-saved
    - Memory management
    - ABI: Application Binary Interface
- Data
    1. Arrays  
        - 1D arrays  
            <img src="Images/arrays.png" width="60%">  
        - Nested 2D arrays: `int A[R][C]`
            <img src="Images/array_nested.png" width="80%">  
        - Multi-level 2D arrays:  
            <img src="Images/array_multilevel.png" width="80%">  
    2. Structs
        - Represented as block of memory
            <img src="Images/struct.png" width="80%">  
        - Fields are ordered according to declaration
        - Alignment:  
            - Within struct: Each element has alignment requirement K, where K is the size of this element
                <img src="Images/alignment.png" width="80%">  
            - Overall: Each struct has alignment requirement K, where K is the largest alignment of any element in struct
                <img src="Images/alignment_overall.png" width="80%">  
            - To save space, put large data types first
    3. Float operations
        - Arguments passed in `%xmm0`, `%xmm1`, ...
        - Result returned in `%xmm0`
        - Different mov instructions are used to move floats
- Address space
    - Currently using 47-bit addresses (highest address of 0x7fffffffffff)
    - Maximum stack size of 8MB on most machines  
        <img src="Images/memory.png" width="60%">  
- Vulnerablities
    1. Buffer overflow
        - Triggered by functions manipulating strings of arbitrary length
        - `gets`, `strcpy`, `strcat`, `scanf`, `fscanf`, `sscanf`
    2. Return-oriented programming (ROT)
        - Make use of "gadgets" in text segment  
        - Trigger with `ret` instruction  
            <img src="Images/rop.png" width="60%">  
- Protection
    1. Use routines limiting string lengths (user-level)
    2. Randomized stack offsets
    3. Nonexecutable code segments
    4. Stack canaries

# Optimizations
- Optimization by programmer
    1. Code motion: Reduce frequency of computations performed   
        <img src="Images/code_motion.png" width="80%">   
        GCC will do this with -O1  
    2. Reduction in strength: Reduce costly operation with simpler one  
        <img src="Images/reduction_in_strength.png" width="80%">  
        Here, int mul requires 3 clock cycles, int add requires 1 clock cycle 
    3. Share common subexpressions  
        <img src="Images/share_common_subexpressions.png" width="80%">  
- Optimization blockers
    1. Procedures: Seen as a "black box"
        - Procedures may have side effects
        - May not return same result with same argument
        - Fix: Use inline functions (GCC with -O1 within single file)
    2. Memory aliasing: Two memory references specify single location
        - The following code does memory load and store every time, because compiler assume possibility of memory aliasing:  
            <img src="Images/memory_aliasing.png" width=80%>  
        - Load and store take multiple clock cycles
        - Easily caused by direct access to storage structures
        - Fix: Define local variable to tell compiler not to check for aliasing
            <img src="Images/aliasing_fix.png" width=60%>  
        - Get in habit of introducing local variables accumulating within loops
- Optimization (by programmer) limitations
    1. Most performed within procedures. Newer versions of GCC do interprocedual optimization, but not between codes in different files
    2. Based on static information
    3. Conservative: Must not change program behavior
- Instruction-level parallelism
    - Superscalar processor: Issue and execute multuple instructions per cycle, and instructions are scheduled dynamically
    - Some instruction have >1 clock cycle latency, but can be pipelined:  
        <img src="Images/pipeline.png" width=70%>  
    - Unrolling
        - Break sequential dependency to break through latency bound (to approach throughput bound)  
            <img src="Images/unrolling.png" width=30%>  
            ```
            for(int i = 0; i < limit; ++i)
                x = x + d[i];
            ```
            can be optimized to:
            ```
            for(int i = 0; i < limit; i += 2)
                x = (x + d[i]) + d[i + 1];
            ```
            but to break sequential dependency:
            ```
            for(int i = 0; i < limit; i += 2)
                x = x + (d[i] + d[i + 1]);
            ``` 
        - adding separate accumulators
    - Branch prediction
        - Backward branches are often loops, predict taken
        - Forward branches are often if, predict not taken
        - Average better than 95% accuracy

# Memory
- Storage technologies
    1. RAMs
        - Volatile: SRAM & DRAM (caches & main memories)
        - Nonvolatile: ROM, PROM, EPROM, EEPROM (firmware, ssd & disk caches)
    2. Rotating disks
    3. SSDs
        - Page can be written only after its block has been erased
- Locality
    - Temporal locality
    - Spatial locality
- Hierarchy  
    <img src="Images/hierarchy.png" width=70%>  
- Caches
    - Each level in hierarchy serves as cache for the level below
    - Types of cache misses
        1. Cold miss: "Warm up" cache
        2. Capacity miss: Working set larger than cache size
        3. Conflict miss: Limited by positioning restrictions imposed by hardware
    - Examples of cache  
        <img src="Images/cache_examples.png" width=70%>  
- Cache memories
    - Concept of locality  
        <img src="Images/locality.png" width=60%>  
    - General organization  
        <img src="Images/address.png" width=20%>
        <img src="Images/organization.png" width=50%>  
        1. Direct mapped cache has (E / associativity = 1)  
            <img src="Images/direct_mapped_cache.png" width=50%>  
        2. E-way set associative cache (Here E / associativity = 2)  
            <img src="Images/e_way_associative_cache.png" width=50%>   
    - Metrics
        1. Miss rate
        2. Hit time
        3. Miss penalty
    - Write cache-friendly code
        1. Make the common cases go first
        2. Minimize the misses in inner loops
        3. Try to maximize spatial locality by reading objects sequentially with stride 1
        4. Try to maximize temporal locality by using an object as often as possible once it's read from memory
    - Example of matrix multiplication
        - In which order to arrange the loops? Do miss rate analysis!
        - It turns out: kij/ikj > ijk/jik > jki/kji
        - Use blocking: multiplying by sub-matrices

# Linking
- Why linkers?
    1. Modularity
    2. Efficiency (separate complilation)
- Two kind of linking
    1. Static linking
    2. Dynamic linking
- What does linker do?
    1. Symbol resolution
        - Functions, `global` vars, `static` vars
        - Definitions are stored in __symbol table__, an array of entries (name, size, location)
        - Three kind of symbols:
            1. Global symbols: non-static functions and non-static vars
            2. External symbols: defined in other modules
            3. Local symbols: static functions and static vars
            - Note: Do not confuse local symbols with local variables. Local variables are allocated in stack at runtime, and have nothing to do with linker. 
        - Symbol resolution
            - Symbols are strong or weak:
                1. Strong: functions and initialized globals
                2. Weak: uninitialized globals
            - Multiple strong symbols are not allowed
            - Choose the strong symbol over weak symbols
            - If there are multiple weak symbols, choose arbitrary one 
                - May cause undefined behavior over different compilers
                - Fix: use `static` and explicit `extern` 
    2. Relocation
        - Merge text and data segment
        - Relative location -> absolute location
        - Updates symbol table
            - Relocation entries are used to aid symbol resolving:  
            `a: R_X86_64_32 array`
- Three kinds of object files
    1. Relocatable object file (.o file)
    2. Executable object file (a.out file)
    3. Shared object file (.so file or .dll file)
- ELF format (Executable and Linkable Format)  
    - All 3 object files use ELF format  
    <img src="Images/elf.png" width=65%>
    <img src="Images/elf_2.png" width=35%>  
- Static libraries (.a archive files)
    - Concatenate related relocatable object files into a single file with an index (called an archive)
    - During linking, only referenced .o files are linked
    - Command line order matters!
        - During scan, keep a list of currently unresolved references
        - If any entries in the unresolved list at end of scan, then error
        - Fix: put libraries at the end of command line
    - Commonly used libraries:
        - `libc.a` (the C standard library)
        - `limb.a` (the C math library)
    - Disadvantages
        - Duplication in storage
        - Bug fixes require relink
        - Fix: shared libraries
- Shared libraries
    - Dynamic linking can happen at:
        1. Load time
            - Handled by the dynamic linker
            - `libc.so` usually dynamically linked
        2. Run time
            - `dlopen()` interface in linux
- Library interpositioning
    - Can happen at:
        1. Compile time
        2. Link time
        3. Load/run time
    - Can be used for:
        1. Detecting memory leaks
        2. Generating address traces
    
# Exception Control Flows (ECF)
- ECFs exists in all levels:
    1. Exceptions (low level)
        - Processor responses to external events
        - Exception tables
    2. Context switch
    3. Signals
    4. Nonlocal jumps
- Exceptions (equivalent to user-kernel transition)  
    <img src="Images/exceptions.png" width=50%>  
    1. Asynchronous (Interrupts)
        - Indicated by INT pin
        - Control flow returns to next instruction
    2. Synchronous
        1. Traps
            - Intentional (syscall, breakpoints)
            - Control flow returns to next instruction
        2. Faults
            - Unintentional but possibly recoverable
            - Control flow returns to current instruction or aborts
        3. Aborts
            - Unintentional and unrecoverable
- Context switches

# Virtual Memory
- Physical Addressing: Used in microcontrollers, embedded systems, etc.
- __Mentality__: Main memory is a fully-associative cache for disk
    - Load doesn't necessarily happen with `execve()`. It only allocates virtual address space with valid bit of 0
    - __Loading is a result of a page fault__ (demand paging)
- Kernel memory invisible to application program. Kernel's address space starts with 1.
- Every memory access go through cache memory:
    - Both memory and cache gets updated after page fault  
        <img src="Images/vm_and_cache.png" width=60%>  
- Address translation: Multi-level page tables
- TLB: Small __set-associative__ hardware cache in MMU
- Works only because of locality

# System-Level I/O
- Unix I/O
    1. Opening and closing files: `open()`, `close()`
    2. Reading and writing files: `read()`, `write()`
    3. Changing file position: `lseek()`
    4. View file metadata: `stat()`
        - `stat()` are both a syscall and a linux program
        - Syscalls are in second section of man: `man 2 stat` 
    - Always check return codes for these syscalls
- File types: Regular, directory, socket, named pipes, symlinks, character and block devices
- Short counts: (`nbytes < sizeof(buf)`) are possible
- Wrapper: RIO (robust I/O) package
    1. Unbuffered I/O of binary data: `rio_readn()` and `rio_writen()`
    2. Buffered I/O of text or binary: `rio_readlineb()` and `rio_readnb()`
    - RIO package is better for input and output on network sockets
- Standard I/O
    1. Opening and closing: `fopen()` and `fclose()`
    2. Reading and writing bytes: `fread()` and `fwrite()`
    3. Reading and writing text lines: `fgets()` and `fputs()`
    4. Formatted reading and writing: `fscanf()` and `fprintf()`
    - C program begin with 3 open files:
        1. `stdin` (descriptor 0) 
        2. `stdout` (descriptor 1)
        3. `stderr` (descriptor 2)
- Trace syscalls with the Linux `strace` program
- Choosing I/O functions
    - General: Use highest-level functions
    - When to use Unix I/O: Signal handlers because unix I/O functions are `async-signal-safe`
    - When to use standard I/O: Disks, terminals
    - When to use RIO: Network sockets
- How kernel represents open files  
    <img src="Images/open_files1.png" width=60%>
    - Open file table: An instance of opening file
        - If a process opens a file twice, there are two open file tables pointing to the same v-node table
    - V-node table: File metadata (regardless of whether file is open)
    - After `fork()`, `refcnt` is incremented:  
        <img src="Images/open_files2.png" width=60%>  
        - Two processes share a same instance of opened file (including file position)
    - `dup2(int oldfd, int newfd)`: Used for I/O redirection
        - After calling `dup2(4, 1)`:  
            <img src="Images/open_files3.png" width=60%>  
- Recommended references:
    - W. Richard Stevens & Stephen A. Rago, _Advanced Programming in the Unix Environment_, 2 nd Edition, Addison Wesley, 2005

# Virtual Memory: Systems
- End-to-end Core i7 Address Translation  
    <img src="Images/address_translation.png" width=80%>  
- L1 d-cache `index` and `offset` have 12 bits is NOT a coincidence: Speed up address translation  
    <img src="Images/cache_speedup.png" width=50%>  
- Linux organizes VM as collections of areas:  
    - Fault handling: Traverse the `vm_area_struct`s to check if page is allocated
    <img src="Images/areas.png" width=80%>  
- Private Copy-on-write (COW)
- Memory Mapping: `void -mmap(void -start, int len, int prot, int flags, int fd, int offset)`
    - `start`: A hint address
    - `prot`: `PROT_READ`, `PROT_WRITE`, `PROT_EXEC` 
    - `flags`: `MAP_ANON`, `MAP_PRIVATE`, `MAP_SHARED`
    - Returns a pointer to the start of mapped area (may not be start)

# Concurrency
- Iterative servers have serious flaws. 
    - Easily get blocked by single misbehaving client
        - Note: Blocking does not happen upon client calling `connect()` or `write()`, but upon `read()`. This is because server's kernel provides buffering
    - So we need concurrent servers
1. Process-based servers
    - Parent __must__ close connected socket (parent doesn't get reaped)
    - Child __should__ close listening socket (child gets reaped)
    - Reap child with `SIGCHLD` handler
2. Event-based servers
    - Manage multiple connections in user space 
    - Determine events using `select()` or `epoll()`
    - Design of choice for high-performance web servers
    - However, hard to provide find-grained concurrency
    - Cannot take advantage of multi-core
3. Thread-based servers
    - Can run threads in `detached` mode. It will run independently, and get reaped automatically
    - Possible race conditions when passing parameters to new thread in `pthread_create()`