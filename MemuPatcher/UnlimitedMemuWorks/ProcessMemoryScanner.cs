using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace UnlimitedMemuWorks
{
    /// <summary>
    /// Class used for scanning a process memory and replacing byte arrays
    /// Thank god for https://www.codeproject.com/Articles/716227/Csharp-How-to-Scan-a-Process-Memory
    /// </summary>
    class ProcessMemoryScanner
    {

        // REQUIRED CONSTS

        const int PROCESS_QUERY_INFORMATION = 0x0400;
        const int MEM_COMMIT = 0x00001000;
        const int PAGE_READWRITE = 0x04;
        const int PROCESS_WM_READ = 0x0010;
        const int PROCESS_WM_WRITE = 0x0020;

        // REQUIRED METHODS

        [DllImport("kernel32.dll")]
        public static extern IntPtr OpenProcess
             (int dwDesiredAccess, bool bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool CloseHandle(IntPtr hObject);

        [DllImport("kernel32.dll")]
        public static extern bool ReadProcessMemory
        (int hProcess, int lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesRead);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory
        (int hProcess, int lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesWritten);

        [DllImport("kernel32.dll")]
        static extern void GetSystemInfo(out SYSTEM_INFO lpSystemInfo);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern int VirtualQueryEx(IntPtr hProcess,
        IntPtr lpAddress, out MEMORY_BASIC_INFORMATION lpBuffer, uint dwLength);


        // REQUIRED STRUCTS

        public enum AllocationProtectEnum : uint
        {
            PAGE_EXECUTE = 0x00000010,
            PAGE_EXECUTE_READ = 0x00000020,
            PAGE_EXECUTE_READWRITE = 0x00000040,
            PAGE_EXECUTE_WRITECOPY = 0x00000080,
            PAGE_NOACCESS = 0x00000001,
            PAGE_READONLY = 0x00000002,
            PAGE_READWRITE = 0x00000004,
            PAGE_WRITECOPY = 0x00000008,
            PAGE_GUARD = 0x00000100,
            PAGE_NOCACHE = 0x00000200,
            PAGE_WRITECOMBINE = 0x00000400
        }

        public enum StateEnum : uint
        {
            MEM_COMMIT = 0x1000,
            MEM_FREE = 0x10000,
            MEM_RESERVE = 0x2000
        }

        public enum TypeEnum : uint
        {
            MEM_IMAGE = 0x1000000,
            MEM_MAPPED = 0x40000,
            MEM_PRIVATE = 0x20000
        }

        public struct MEMORY_BASIC_INFORMATION
        {
            public IntPtr BaseAddress;
            public IntPtr AllocationBase;
            public AllocationProtectEnum AllocationProtect;
            public IntPtr RegionSize;
            public StateEnum State;
            public AllocationProtectEnum Protect;
            public TypeEnum Type;
        }

        public struct SYSTEM_INFO
        {
            public ushort processorArchitecture;
            ushort reserved;
            public uint pageSize;
            public IntPtr minimumApplicationAddress;
            public IntPtr maximumApplicationAddress;
            public IntPtr activeProcessorMask;
            public uint numberOfProcessors;
            public uint processorType;
            public uint allocationGranularity;
            public ushort processorLevel;
            public ushort processorRevision;
        }

        //Parameters
        private readonly IntPtr proc_min_address;
        private readonly IntPtr proc_max_address;
        private Process toScan;

        public ProcessMemoryScanner(Process p)
        {
            // getting minimum & maximum address of apps
            SYSTEM_INFO sys_info = new SYSTEM_INFO();
            GetSystemInfo(out sys_info);

            proc_min_address = sys_info.minimumApplicationAddress;
            proc_max_address = sys_info.maximumApplicationAddress;



            toScan = p;
        }

        /// <summary>
        /// Replace the requested byte arrays in process RAM. Returns the number of replaced strings.
        /// </summary>
        /// <param name="source"></param>
        /// <param name="replacement"></param>
        /// <returns></returns>
        public int ReplaceInRAM(byte[] source, byte[] replacement)
        {
            int replaced = 0;

            // saving the values as long ints so I won't have to do a lot of casts later
            uint proc_index_address_l = (uint)proc_min_address;
            uint proc_max_address_l = (uint)proc_max_address;

            //The address we'll use and modify while parsing the process
            IntPtr proc_index_address = proc_min_address;

            // opening the process with desired access level
            IntPtr processHandle =
            OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_WM_READ | PROCESS_WM_WRITE, false, toScan.Id);

            // this will store any information we get from VirtualQueryEx()
            MEMORY_BASIC_INFORMATION mem_basic_info = new MEMORY_BASIC_INFORMATION();

            int bytesRead = 0;  // number of bytes read with ReadProcessMemory

            //Go through the entire process memory 
            while (proc_index_address_l < proc_max_address_l)
            {
                // 28 = sizeof(MEMORY_BASIC_INFORMATION)
                //Scan this chunk of memory and store info about it in membasicinfo
                VirtualQueryEx(processHandle, proc_index_address, out mem_basic_info, 28);

                // if this memory chunk is accessible
                if (mem_basic_info.Protect == AllocationProtectEnum.PAGE_READWRITE && mem_basic_info.State == StateEnum.MEM_COMMIT)
                {
                    byte[] buffer = new byte[(uint)mem_basic_info.RegionSize];

                    // read everything in the buffer above
                    ReadProcessMemory((int)processHandle,
                    (int)mem_basic_info.BaseAddress, buffer, (int)mem_basic_info.RegionSize, ref bytesRead);

                    // then look at it carefully for the first byte of the source - if it's present, we'll do a more accurate lookup.
                    for (int i = 0; i < (uint)mem_basic_info.RegionSize; i++)
                    {
                        if (buffer[i] == source[0] && i + source.Length < buffer.Length)
                        {
                            Boolean equal = true;

                            for (int j = 0; j < source.Length; j++) //check if the entirety of the source fits
                            {
                                equal = buffer[i + j] == source[j];
                                if (!equal)
                                    break;
                            }

                            if (equal)
                            {
                                Console.WriteLine("Match found at value 0x" + (mem_basic_info.BaseAddress + i).ToString("X") + "!");

                                byte[] testedArray = new byte[source.Length];
                                Array.Copy(buffer, i, testedArray, 0, source.Length);


                                //replacin time
                                int bytesWritten = 0;
                                WriteProcessMemory((int)processHandle, (int)mem_basic_info.BaseAddress + i, replacement, replacement.Length, ref bytesWritten);
                                Console.WriteLine("Replaced " + bytesWritten + " bytes ");
                                replaced++;
                            }
                        }
                    }
                }

                uint oldval = proc_index_address_l;

                // move to the next memory chunk
                proc_index_address_l += (uint)mem_basic_info.RegionSize;


                if (proc_index_address_l < proc_max_address_l)
                    proc_index_address = new IntPtr(proc_index_address_l);

                //In some cases, the RegionSize ends up being negative (mostly seen on win7)
                //As this basically means we reached the end earlier than proc_max_address, let's just consider the job done.
                if (oldval > proc_index_address_l)
                {
                    break;
                }

            }

            CloseHandle(processHandle);
            return replaced;
        }

    }
}
