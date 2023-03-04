#include <stdint.h>

typedef uint16_t CHAR16;

typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct {
    EFI_TABLE_HEADER Hdr;
    CHAR16* FirmwareVendor;
    UINT32 FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
    EFI_HANDLE StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr;
    EFI_RUNTIME_SERVICES* RuntimeServices;
    EFI_BOOT_SERVICES* BootServices;
    UINTN NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE* ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];
} EFI_GUID;

typedef struct {
    UINT32 Attributes;
    EFI_GUID VendorGuid;
    UINTN DataSize;
    VOID* Data;
} EFI_VARIABLE;

typedef enum {
    EFI_SUCCESS = 0,
    EFI_BUFFER_TOO_SMALL = 5,
    EFI_INVALID_PARAMETER = 31,
    EFI_NOT_FOUND = 14
} EFI_STATUS;

typedef EFI_STATUS (EFIAPI *EFI_GET_VARIABLE)(
    IN CHAR16* VariableName,
    IN EFI_GUID* VendorGuid,
    OUT UINT32* Attributes OPTIONAL,
    IN OUT UINTN* DataSize,
    OUT VOID* Data OPTIONAL
);

EFI_STATUS GetVariable(
    IN CHAR16* VariableName,
    IN EFI_GUID* VendorGuid,
    OUT UINT32* Attributes OPTIONAL,
    IN OUT UINTN* DataSize,
    OUT VOID* Data OPTIONAL
) {
    EFI_SYSTEM_TABLE* SystemTable;
    EFI_GET_VARIABLE GetVariable;

    SystemTable = (EFI_SYSTEM_TABLE*) 0xFFFFFFFFFFFFF000;
    GetVariable = SystemTable->RuntimeServices->GetVariable;

    return GetVariable(
        VariableName,
        VendorGuid,
        Attributes,
        DataSize,
        Data
    );
}

int main() {
    EFI_GUID vendor_guid = { 0x8be4df61, 0x93ca, 0x11d2, { 0xaa, 0x0d, 0x00, 0xe0, 0x98, 0x03, 0x2b, 0x8c } };
    CHAR16 var_name[] = L"my_var_name";
    UINTN var_size = 0;
    VOID* var_data = NULL;

    EFI_STATUS status = GetVariable(
        var_name,
        &vendor_guid,
        NULL,
        &var_size,
        var_data
    );

    if (status == EFI_BUFFER_TOO_SMALL) {
        var_data = malloc(var_size);
        status = GetVariable(
            var_name,
            &vendor_guid,
            NULL,
            &var_size,
            var_data
        );

        if (status == EFI_SUCCESS) {
            // Variable read successfully
            Print(L"This variable is: %s\n", var_name);
        } else {
            // Error reading variable
        }

        free(var_data);
    } else {
        // Error getting variable size
    }

    return 0;
}
