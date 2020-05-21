// Hardware.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>
#include <msclr\marshal_cppstd.h>
using namespace System;
using namespace System::Management;
using namespace System::IO;
using namespace msclr::interop;

class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // getters
    static std::string Vendor(void) { return CPU_Rep.vendor_; }
    static std::string Brand(void) { return CPU_Rep.brand_; }

    static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
    static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
    static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
    static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
    static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
    static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
    static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
    static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
    static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
    static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
    static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
    static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
    static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
    static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
    static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
    static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }

    static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
    static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
    static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
    static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
    static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
    static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
    static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
    static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
    static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }

    static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
    static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
    static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
    static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
    static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
    static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
    static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
    static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
    static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
    static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
    static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
    static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
    static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
    static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
    static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

    static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

    static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
    static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
    static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
    static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
    static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
    static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

    static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
    static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
    static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
    static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
    static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal()
            : nIds_{ 0 },
            nExIds_{ 0 },
            isIntel_{ false },
            isAMD_{ false },
            f_1_ECX_{ 0 },
            f_1_EDX_{ 0 },
            f_7_EBX_{ 0 },
            f_7_ECX_{ 0 },
            f_81_ECX_{ 0 },
            f_81_EDX_{ 0 },
            data_{},
            extdata_{}
        {
            //int cpuInfo[4] = {-1};
            std::array<int, 4> cpui;

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
            nIds_ = cpui[0];

            for (int i = 0; i <= nIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                data_.push_back(cpui);
            }

            // Capture vendor string
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
            *reinterpret_cast<int*>(vendor) = data_[0][1];
            *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
            *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
            vendor_ = vendor;
            if (vendor_ == "GenuineIntel")
            {
                isIntel_ = true;
            }
            else if (vendor_ == "AuthenticAMD")
            {
                isAMD_ = true;
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_ECX_ = data_[1][2];
                f_1_EDX_ = data_[1][3];
            }

            // load bitset with flags for function 0x00000007
            if (nIds_ >= 7)
            {
                f_7_EBX_ = data_[7][1];
                f_7_ECX_ = data_[7][2];
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
            __cpuid(cpui.data(), 0x80000000);
            nExIds_ = cpui[0];

            char brand[0x40];
            memset(brand, 0, sizeof(brand));

            for (int i = 0x80000000; i <= nExIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                extdata_.push_back(cpui);
            }

            // load bitset with flags for function 0x80000001
            if (nExIds_ >= 0x80000001)
            {
                f_81_ECX_ = extdata_[1][2];
                f_81_EDX_ = extdata_[1][3];
            }

            // Interpret CPU brand string if reported
            if (nExIds_ >= 0x80000004)
            {
                memcpy(brand, extdata_[2].data(), sizeof(cpui));
                memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
                memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
                brand_ = brand;
            }
        };

        int nIds_;
        int nExIds_;
        std::string vendor_;
        std::string brand_;
        bool isIntel_;
        bool isAMD_;
        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;
        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;
        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
    };
};

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

void CPUID(StreamWriter^% sw) {
    auto& outstream = std::cout;

    auto support_message = [&outstream](std::string isa_feature, bool is_supported, StreamWriter^% sw) {
        Console::Write("{0,33}", " ");
        outstream << isa_feature << (is_supported ? " supported" : " not supported") << std::endl;
        sw->Write("{0,30}", " ");
        sw->Write(marshal_as<String^>(isa_feature));
        sw->Write(is_supported ? " supported\n" : " not supported\n");
    };
    Console::Write("Instructions:\n");
    sw->WriteLine("Instructions:");
    support_message("3DNOW", InstructionSet::_3DNOW(), sw);
    support_message("3DNOWEXT", InstructionSet::_3DNOWEXT(), sw);
    support_message("ABM", InstructionSet::ABM(), sw);
    support_message("ADX", InstructionSet::ADX(), sw);
    support_message("AES", InstructionSet::AES(), sw);
    support_message("AVX", InstructionSet::AVX(), sw);
    support_message("AVX2", InstructionSet::AVX2(), sw);
    support_message("AVX512CD", InstructionSet::AVX512CD(), sw);
    support_message("AVX512ER", InstructionSet::AVX512ER(), sw);
    support_message("AVX512F", InstructionSet::AVX512F(), sw);
    support_message("AVX512PF", InstructionSet::AVX512PF(), sw);
    support_message("BMI1", InstructionSet::BMI1(), sw);
    support_message("BMI2", InstructionSet::BMI2(), sw);
    support_message("CLFSH", InstructionSet::CLFSH(), sw);
    support_message("CMPXCHG16B", InstructionSet::CMPXCHG16B(), sw);
    support_message("CX8", InstructionSet::CX8(), sw);
    support_message("ERMS", InstructionSet::ERMS(), sw);
    support_message("F16C", InstructionSet::F16C(), sw);
    support_message("FMA", InstructionSet::FMA(), sw);
    support_message("FSGSBASE", InstructionSet::FSGSBASE(), sw);
    support_message("FXSR", InstructionSet::FXSR(), sw);
    support_message("HLE", InstructionSet::HLE(), sw);
    support_message("INVPCID", InstructionSet::INVPCID(), sw);
    support_message("LAHF", InstructionSet::LAHF(), sw);
    support_message("LZCNT", InstructionSet::LZCNT(), sw);
    support_message("MMX", InstructionSet::MMX(), sw);
    support_message("MMXEXT", InstructionSet::MMXEXT(), sw);
    support_message("MONITOR", InstructionSet::MONITOR(), sw);
    support_message("MOVBE", InstructionSet::MOVBE(), sw);
    support_message("MSR", InstructionSet::MSR(), sw);
    support_message("OSXSAVE", InstructionSet::OSXSAVE(), sw);
    support_message("PCLMULQDQ", InstructionSet::PCLMULQDQ(), sw);
    support_message("POPCNT", InstructionSet::POPCNT(), sw);
    support_message("PREFETCHWT1", InstructionSet::PREFETCHWT1(), sw);
    support_message("RDRAND", InstructionSet::RDRAND(), sw);
    support_message("RDSEED", InstructionSet::RDSEED(), sw);
    support_message("RDTSCP", InstructionSet::RDTSCP(), sw);
    support_message("RTM", InstructionSet::RTM(), sw);
    support_message("SEP", InstructionSet::SEP(), sw);
    support_message("SHA", InstructionSet::SHA(), sw);
    support_message("SSE", InstructionSet::SSE(), sw);
    support_message("SSE2", InstructionSet::SSE2(), sw);
    support_message("SSE3", InstructionSet::SSE3(), sw);
    support_message("SSE4.1", InstructionSet::SSE41(), sw);
    support_message("SSE4.2", InstructionSet::SSE42(), sw);
    support_message("SSE4a", InstructionSet::SSE4a(), sw);
    support_message("SSSE3", InstructionSet::SSSE3(), sw);
    support_message("SYSCALL", InstructionSet::SYSCALL(), sw);
    support_message("TBM", InstructionSet::TBM(), sw);
    support_message("XOP", InstructionSet::XOP(), sw);
    support_message("XSAVE", InstructionSet::XSAVE(), sw);
}

void printHardwareInfo(String^ hardwareClass, array<System::String^>^ property, String^ direction, StreamWriter^ %sw, String^ description)
{
    ManagementObjectSearcher^ searcher = gcnew ManagementObjectSearcher("root\\"+direction, "SELECT * FROM " + hardwareClass);
    ManagementObjectCollection^ collection = searcher->Get();
    Console::Write(description);
    sw->Write(description);
    int len = description->Length;
    for each (String ^ element in property) {
        Console::Write(element + ":\n");
        sw->Write(element + ":\n");
        for each (ManagementObject ^ object in collection)
        {
            Console::Write("{0,33}", " ");
            Console::Write(object[element]+"\n");
            sw->Write("{0,30}", " ");
            sw->Write(object[element] + "\n");
        }
        Console::Write("\n");
        sw->Write("\n");
    }
}

int main()
{
    String^ filename = "D:\\abc.txt";
    StreamWriter^ sw = gcnew StreamWriter(filename);


    //Disk partition info
    //array<System::String^> ^propertiesDiskPartition = gcnew array<String^>{"Name", "BlockSize", "Bootable", "BootPartition", "Caption", "Description",
    //                                                                       "DeviceID", "DiskIndex", "Index", "NumberOfBlocks", "PrimaryPartition",
    //                                                                       "RewritePartition", "Size", "StartingOffset", "SystemName", "Type"};
    //array<System::String^>^ propertiesLogicalDisk = gcnew array<String^>{"DeviceId", "Description", "Access", "FreeSpace", "Size", "Compressed", 
    //                                                                     "DriveType", "FileSystem", "MaximumComponentLength", "MediaType", 
    //                                                                     "SupportsDiskQuotas", "SupportsFileBasedCompression", "VolumeSerialNumber"};
    //printHardwareInfo("Win32_DiskPartition", propertiesDiskPartition, "CIMV2", sw, "DISK PARTITION INFO\n");
    //printHardwareInfo("Win32_LogicalDisk", propertiesLogicalDisk, "CIMV2", sw, "");

    ////BIOS info
    //array<System::String^>^ propertiesBIOS = gcnew array<String^>{"SMBIOSBIOSVersion", "CurrentLanguage", "Description", "EmbeddedControllerMajorVersion", 
    //                                                              "EmbeddedControllerMinorVersion", "InstallableLanguages", "Manufacturer", "Name",
    //                                                              "PrimaryBIOS", "ReleaseDate", "SerialNumber", "SMBIOSMajorVersion", "SMBIOSMinorVersion", 
    //                                                              "SMBIOSPresent", "SoftwareElementID", "SoftwareElementState", "Status", "SystemBiosMajorVersion",
    //                                                              "SystemBiosMinorVersion", "TargetOperatingSystem", "Version"};

    //printHardwareInfo("Win32_BIOS", propertiesBIOS, "CIMV2", sw, "\nBIOS INFO\n");
    //Processor info
    //array<System::String^>^ propertiesCPU = gcnew array<String^>{"Name", "Description", "AddressWidth", "Architecture", "Availability", "Characteristics", "CpuStatus", "CurrentClockSpeed",
    //                                                             "CurrentVoltage", "DataWidth", "DeviceID", "ExtClock", "Family", "L2CacheSize", "L2CacheSpeed", 
    //                                                             "L3CacheSize", "L3CacheSpeed", "Level", "LoadPercentage", "Manufacturer", "MaxClockSpeed", "NumberOfCores", 
    //                                                             "NumberOfEnabledCore", "NumberOfLogicalProcessors", "PartNumber", "PowerManagementSupported", "ProcessorId", 
    //                                                             "ProcessorType", "Revision", "Role", "SecondLevelAddressTranslationExtensions", "SocketDesignation", "Status", 
    //                                                             "StatusInfo", "SystemName", "ThreadCount", "UpgradeMethod", "VirtualizationFirmwareEnabled", "VMMonitorModeExtensions"};

    //printHardwareInfo("Win32_Processor", propertiesCPU, "CIMV2", sw, "\nPROCESSOR INFO\n");
    //CPUID(sw);
    //Monitor info
    //array<System::String^>^ propertiesMonitor1 = gcnew array<String^>{"DeviceID", "PNPDeviceID", "Availability", "ConfigManagerErrorCode", "ConfigManagerUserConfig", 
    //                                                                    "Description", "MonitorManufacturer", "MonitorType", "PixelsPerXLogicalInch", "PixelsPerYLogicalInch",
    //                                                                    "ScreenHeight", "ScreenWidth", "Status", "SystemName"};
    //array<System::String^>^ propertiesMonitor2 = gcnew array<String^>{"Active", "DisplayTransferCharacteristic", "InstanceName", "MaxHorizontalImageSize",
    //                                                                  "MaxVerticalImageSize", "SupportedDisplayFeatures", "VideoInputType"};
    //array<System::String^>^ propertiesMonitor3 = gcnew array<String^>{"CurrentBrightness", "Levels"};

    //printHardwareInfo("Win32_DesktopMonitor", propertiesMonitor1, "CIMV2", sw, "\nMONITOR INFO\n");
    //printHardwareInfo("WmiMonitorBasicDisplayParams", propertiesMonitor2, "WMI", sw, "");
    //printHardwareInfo("WmiMonitorBrightness", propertiesMonitor3, "WMI", sw, "");
    ////HDD info
    //array<System::String^>^ propertiesHDD = gcnew array<String^>{"Model", "BytesPerSector", "ConfigManagerErrorCode", "ConfigManagerUserConfig",
    //                                                             "Description", "DeviceID", "FirmwareRevision", "Index", "InterfaceType", "Manufacturer",
    //                                                             "MediaLoaded", "MediaType", "Name", "Partitions", "PNPDeviceID", "SCSIBus", "Size",
    //                                                             "SCSILogicalUnit", "SCSIPort", "SCSITargetId", "SectorsPerTrack", "SerialNumber", "Signature",
    //                                                             "Status", "SystemName", "TotalCylinders", "TotalHeads", "TotalSectors", "TotalTracks", "TracksPerCylinder"};

    //printHardwareInfo("Win32_DiskDrive", propertiesHDD, "CIMV2", sw, "\nHDD INFO\n");
    ////Video Controller info
    //array<System::String^>^ propertiesVC = gcnew array<String^>{"Name", "Description", "AdapterCompatibility", "AdapterDACType", "AdapterRAM", "Availability", "ConfigManagerErrorCode",
    //                                                            "ConfigManagerUserConfig", "CurrentBitsPerPixel", "CurrentHorizontalResolution", "CurrentNumberOfColors", 
    //                                                            "CurrentNumberOfColumns", "CurrentNumberOfRows", "CurrentRefreshRate", "CurrentScanMode", 
    //                                                            "CurrentVerticalResolution", "DeviceID", "DitherType", "DriverDate", "DriverVersion", 
    //                                                            "InfFilename", "InfSection", "InstalledDisplayDrivers", "MaxRefreshRate", "MinRefreshRate", "Monochrome", 
    //                                                            "PNPDeviceID", "Status", "SystemName", "VideoArchitecture", "VideoMemoryType", "VideoModeDescription", 
    //                                                            "VideoProcessor", };
    //printHardwareInfo("Win32_VideoController", propertiesVC, "CIMV2", sw, "\nVIDEOCONTROLLER INFO\n");
    ////Network Adapter info
    //array<System::String^>^ propertiesNA = gcnew array<String^>{"AdapterType", "AdapterTypeID", "Availability", "Caption", "ConfigManagerErrorCode", 
    //                                                            "ConfigManagerUserConfig", "Description", "DeviceID", 
    //                                                            "GUID", "Index", "Installed", "InterfaceIndex", "MACAddress", "Manufacturer", 
    //                                                            "MaxNumberControlled", "Name", "NetConnectionID", "NetConnectionStatus", "NetEnabled", 
    //                                                            "PhysicalAdapter", "PNPDeviceID", "PowerManagementSupported", 
    //                                                            "ProductName", "ServiceName", "Speed", "SystemName", "TimeOfLastReset"};

    //printHardwareInfo("Win32_NetworkAdapter", propertiesNA, "CIMV2", sw, "\nNETWORK ADAPTER INFO\n");
    ////Process info
    //array<System::String^>^ propertiesProcess = gcnew array<String^>{"Name", "CommandLine", "CreationDate", "CSName", "Description",
    //                                                                 "ExecutablePath", "Handle", "HandleCount", "KernelModeTime", "MaximumWorkingSetSize", 
    //                                                                 "MinimumWorkingSetSize", "OSName", "OtherOperationCount", "OtherTransferCount", 
    //                                                                 "PageFaults", "PageFileUsage", "ParentProcessId", "PeakPageFileUsage", "PeakVirtualSize", "PeakWorkingSetSize", 
    //                                                                 "Priority", "PrivatePageCount", "ProcessId", "QuotaNonPagedPoolUsage", "QuotaPagedPoolUsage", 
    //                                                                 "QuotaPeakNonPagedPoolUsage", "QuotaPeakPagedPoolUsage", "ReadOperationCount", "ReadTransferCount", "SessionId", 
    //                                                                 "ThreadCount", "UserModeTime", "VirtualSize", "WindowsVersion", "WorkingSetSize", 
    //                                                                 "WriteOperationCount", "WriteTransferCount"};
    //printHardwareInfo("Win32_Process", propertiesProcess, "CIMV2", sw, "\nPROCESS INFO\n");
    //Motherboard info
    //array<System::String^>^ propertiesBaseBoard = gcnew array<String^>{"Caption", "Description", "HostingBoard", "HotSwappable", "Manufacturer", "Name", "PoweredOn", "Product", 
    //                                                                   "Removable", "Replaceable", "RequiresDaughterBoard", "SerialNumber", "Status", "Tag", "Version"};
    //printHardwareInfo("Win32_BaseBoard", propertiesBaseBoard, "CIMV2", sw, "\nMOTHERBOARD INFO\n");

    //array<System::String^>^ propertiesOS = gcnew array<String^>{"BootDevice", "BuildNumber", "BuildType", "Caption", "CodeSet", "CountryCode", 
    //                                                            "CSName", "CurrentTimeZone", "DataExecutionPrevention_Available", 
    //                                                            "DataExecutionPrevention_32BitApplications", "DataExecutionPrevention_Drivers", "DataExecutionPrevention_SupportPolicy", 
    //                                                            "Debug", "Distributed", "EncryptionLevel", "ForegroundApplicationBoost", "FreePhysicalMemory", 
    //                                                            "FreeSpaceInPagingFiles", "FreeVirtualMemory", "InstallDate", "LastBootUpTime", "LocalDateTime", 
    //                                                            "Locale", "Manufacturer", "MaxNumberOfProcesses", "MaxProcessMemorySize", "Name",  
    //                                                            "NumberOfProcesses", "NumberOfUsers", "OperatingSystemSKU", "OSArchitecture", "OSLanguage", "OSProductSuite", 
    //                                                            "OSType", "PortableOperatingSystem", "Primary", 
    //                                                            "ProductType", "RegisteredUser", "SerialNumber", "ServicePackMajorVersion", "ServicePackMinorVersion", 
    //                                                            "SizeStoredInPagingFiles", "Status", "SuiteMask", "SystemDevice", "SystemDirectory", "SystemDrive", 
    //                                                            "TotalVirtualMemorySize", "TotalVisibleMemorySize", "Version", "WindowsDirectory"};
    //printHardwareInfo("Win32_OperatingSystem", propertiesOS, "CIMV2", sw, "\nOS INFO\n");

    //Keyboard info
    array<System::String^>^ propertiesKeyboard = gcnew array<String^>{"Name", "ConfigManagerErrorCode", "ConfigManagerUserConfig",
                                                                      "Description", "DeviceID",  "Layout", "NumberOfFunctionKeys", 
                                                                      "PNPDeviceID", "PowerManagementSupported", "Status"};
    printHardwareInfo("Win32_Keyboard", propertiesKeyboard, "CIMV2", sw, "\nKEYBOARD INFO\n");
    //Mouse info
    array<System::String^>^ propertiesMouse = gcnew array<String^>{"Name", "ConfigManagerErrorCode", "ConfigManagerUserConfig",
                                                                   "Description", "DeviceID", "PNPDeviceID", "PowerManagementSupported", 
                                                                   "Status"};
    printHardwareInfo("Win32_PointingDevice", propertiesMouse, "CIMV2", sw, "\nMOUSE INFO\n");
    sw->Close();

}
