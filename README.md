  Utility to detect duplicate files
  
  -h [ --help ]            help
  -i [ --iDir ] arg        directories for scanning files
  -e [ --eDir ] arg        directories to exclude from scanning
  -l [ --level ] arg (=0)  scanning level (one for all directories, 0 - only 
                           the specified one directory without subfolders)
  -m [ --min ] arg (=1)    minimum file size for scanning in bytes
  --masks arg              masks of file names allowed for comparison (do not 
                           depend on register)
  -S [ --size ] arg (=1)   block size used to read files
  -H [ --hash ] arg (=md5) hashing algorithms: crc32, md5

Usage examples:
./bayan -h
./bayan -i "./" -l 2 -m 1 --masks "bay(.*)"
./bayan -i "../" -S 8 -l 3 -m 1 --masks "test_test(.*)"
./bayan -i "./" -e "./1/2" './CMakeFiles' -l 6