import os

# Specify the directory path
directory = 'PACE/hidden/'

# Get list of files in the directory
files = os.listdir(directory)

# Print the list of files
print("Files in", directory, ":")
for file_name in files:
    file_path = directory + file_name
    str_file_number = file_name.split('.')[0]
    print("str_file_number:", str_file_number)

    with open(file_path, 'r') as file:
        # Read each line from the file

        str_node_map = {}
        number_of_nodes = 0

        edge_list = []

        ctr = 0
        for line in file:
            # Process the line here
            str_edge = line.split()
            #print(str_edge)  # Print or process the line as needed
            str_node1 = str_edge[0]
            str_node2 = str_edge[1]

            if ( not (str_node1 in str_node_map) ):
                str_node_map[str_node1] = number_of_nodes
                number_of_nodes += 1

            if ( not (str_node2 in str_node_map) ):
                str_node_map[str_node2] = number_of_nodes
                number_of_nodes += 1
            
            edge_list.append( [str_node_map[str_node1], str_node_map[str_node2]] )
        
        #print(edge_list)

        output_file_folder = "input_data/"
        output_file_name = "pace_hidden_" + str_file_number + ".txt"
        output_file_path = output_file_folder + output_file_name

        with open(output_file_path, 'w') as file:
            # Write content to the file
            first_str = str(number_of_nodes) + " " + str(len(edge_list)) + "\n"
            file.write(first_str)

            for edge in edge_list:
                edge_str = str(edge[0]) + " " + str(edge[1]) + "\n"
                file.write(edge_str)
