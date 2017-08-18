/* Copyright 2017 The Apollo Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
=========================================================================*/

#include "modules/map/hdmap/adapter/opendrive_adapter.h"
#include "modules/common/util/string_util.h"
#include "modules/common/util/file.h"
#include "modules/map/proto/map.pb.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "missing file path." << std::endl;
    return -1;
  }
  std::string map_filename = argv[1];
  apollo::hdmap::Map pb_map;
  apollo::hdmap::adapter::OpendriveAdapter opendrive_adapter;
  if (opendrive_adapter.load_data(map_filename, &pb_map) != 0) {
    std::cout << "fail to load data" << std::endl;
    return -1;
  }

  std::string output_ascii_file = map_filename + ".txt";
  apollo::common::util::SetProtoToASCIIFile(pb_map, output_ascii_file);

  std::string output_bin_file = map_filename + ".bin";
  std::fstream output(output_bin_file.c_str(),
                    std::ios::out | std::ios::binary);
  if (!output) {
    std::string err_msg = "fail to open " + output_bin_file;
    std::cout << err_msg << std::endl;
    return false;
  }

  if (!pb_map.SerializeToOstream(&output)) {
    std::string err_msg = "fail to parse " + output_bin_file;
    std::cout << err_msg << std::endl;
    return false;
  }
  output.close();

  pb_map.Clear();
  using apollo::common::util::GetProtoFromFile;
  if (!GetProtoFromFile(output_bin_file, &pb_map)) {
    std::cout << "load map fail" << std::endl;
    return -1;
  }

  std::cout << "load map success" << std::endl;
  return 0;
}
