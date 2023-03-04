#include "efi_library.h"
#include <efi.h>
#include <efilib.h>

/*
int example() {
  // Inicializar el sistema EFI
  InitializeLib(NULL, NULL);
  
  // Iterar sobre todas las variables de UEFI
  EFI_GUID guid;
  UINTN size = 0;
  CHAR16* name = NULL;
  EFI_STATUS status;
  while (1) {
    size = 0;
    status = gRT->GetNextVariableName(&size, name, &guid);
    if (status == EFI_BUFFER_TOO_SMALL) {
      name = AllocatePool(size);
      status = gRT->GetNextVariableName(&size, name, &guid);
      if (EFI_ERROR(status)) {
        // Manejar el error
        FreePool(name);
        return 1;
      }
      // Imprimir el nombre y GUID de la variable
      Print(L"Variable: %s\n", name);
      Print(L"  GUID:   %g\n", &guid);
      FreePool(name);
    } else {
      break;
    }
  }
  
  // Finalizar el sistema EFI
  return 0;
}
*/

int efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table)
{
    EFI_STATUS status;
    UINTN var_size = 0;
    UINT8 *var_value = NULL;
    CHAR16 var_name[64] = { L'm', L'y', L'_', L'v', L'a', L'r', L'_', L'n', L'a', L'm', L'e', L'\0' };

    // Get a pointer to the runtime services table
    EFI_RUNTIME_SERVICES *rt = system_table->RuntimeServices;

    // Get the variable size
    status = rt->GetVariable(var_name, &gEfiGlobalVariableGuid, NULL, &var_size, NULL);
    if (status == EFI_BUFFER_TOO_SMALL)
    {
        // Allocate memory for the variable value
        var_value = (UINT8*)AllocatePool(var_size);
        if (var_value == NULL)
        {
            Print(L"Failed to allocate memory for variable value\n");
            return EFI_OUT_OF_RESOURCES;
        }

        // Read the variable value
        status = rt->GetVariable(var_name, &gEfiGlobalVariableGuid, NULL, &var_size, var_value);
        if (status == EFI_SUCCESS)
        {
            // Print the variable value
            Print(L"The value of my_var_name is: %s\n", var_value);
        }
        else
        {
            Print(L"Failed to read variable value (status=%d)\n", status);
        }

        // Free memory used for variable value
        FreePool(var_value);
    }
    else if (status == EFI_NOT_FOUND)
    {
        Print(L"The variable my_var_name was not found\n");
    }
    else
    {
        Print(L"Failed to get variable size (status=%d)\n", status);
    }

    return EFI_SUCCESS;
}
