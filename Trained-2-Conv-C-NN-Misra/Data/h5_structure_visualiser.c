#include <stdio.h>
#include <stdlib.h>
#include <hdf5.h>
#define MAX_NAME_SIZE 256

/* Created:       25.05.2023
   Last modified: 25.05.2023
   @ Jukka J
*/

/* Not MISRA C:2012 compatible function to to visualize structure of .h5-file.
   Useful in development stage, not needed by actual program.
*/
void print_dataset_structure(hid_t loc_id, const char* name, int indent_level) {
    /* Creating an indentation string based on the indent_level.
       Basically just empty space for printing purposes.
    */
    char indent[4 * indent_level + 1];
    for (int i = 0; i < 4 * indent_level; i++) {
        indent[i] = ' ';
    }
    indent[4 * indent_level] = '\0';
    
    /* Open the object in the HDF5 file */
    hid_t obj_id = H5Oopen(loc_id, name, H5P_DEFAULT);
    if (obj_id >= 0) {
        H5O_info_t obj_info;
        /* Getting information about the object */
        if (H5Oget_info(obj_id, &obj_info) >= 0) {
            /* Check if the object is a group type */
            if (obj_info.type == H5O_TYPE_GROUP) {
                printf("%sGroup: %s\n", indent, name);
                /* Open the group in the HDF5 file */
                hid_t group_id = H5Gopen(loc_id, name, H5P_DEFAULT);
                if (group_id >= 0) {
                    H5G_info_t group_info;
                    /* Get information about the group */
                    if (H5Gget_info(group_id, &group_info) >= 0) {
                        /* Iterate over the links in the group */
                        for (int i = 0; i < group_info.nlinks; i++) {
                            char link_name[MAX_NAME_SIZE];
                            /* Get the name of the link by index */
                            ssize_t link_name_size = H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_NATIVE, i, link_name, MAX_NAME_SIZE, H5P_DEFAULT);
                            if (link_name_size >= 0) {
                                /* Recursively call the function to print the structure of the linked object */
                                print_dataset_structure(group_id, link_name, indent_level + 1);
                            }
                        }
                    }
                     /* Close the group */
                    H5Gclose(group_id);
                }
              /* Check if the object is a dataset */
            } else if (obj_info.type == H5O_TYPE_DATASET) {
                printf("%sDataset: %s\n", indent, name);
            }
        }
        /* Close the object */
        H5Oclose(obj_id);
    }
}

int main() {
    const char* file_path = "./CNN_450-2450-5-550_1110-11-110-111-000_20220330113805.h5";

    hid_t file_id = H5Fopen(file_path, H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        printf("Error: Unable to open file '%s'.\n", file_path);
        return 1;
    }

    print_dataset_structure(file_id, "/", 0);

    H5Fclose(file_id);

    return 0;
}

