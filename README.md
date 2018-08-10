Mesh Processing Tool

Ryan Needham

2018

    Building & Usage
    - running build.sh will compile the source and output a MeshTool executable
    - run the MeshTool program passing a number of meshes, n, and n paths to meshes 
    - NOTE: obj model paths are passed without the file extention
    - the following usages would validate and repair the meshes and output .mesh
      files to the same location the obj files were read from

        $ ./MeshTool 1 Sphere

        $ ./MeshTool 4 models/bust_0 models/bust_1 models/bust_2 models/bust_3
