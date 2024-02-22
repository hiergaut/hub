/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/09/28

#pragma once

#include <filesystem>

#include "InputSensor.hpp"
#include "OutputSensor.hpp"
#include "SensorSpec.hpp"

namespace hub {
namespace sensor {
    namespace utils {

        class SpecAcqs {
        public:
            SpecAcqs(const SpecAcqs&) = delete;
            SpecAcqs(SpecAcqs&&) = default;
            SpecAcqs(const hub::sensor::SensorSpec& sensorSpec)
                : m_sensorSpec { sensorSpec } {};
            hub::sensor::SensorSpec m_sensorSpec;
            std::vector<hub::sensor::Acquisition> m_acqs;
        };

        template <class... InputT>
        std::vector<SpecAcqs> synchonizeInputs(InputT&... inputs)
        {
            std::vector<SpecAcqs> specAcqs;

            for (auto input : { inputs... }) {
                hub::sensor::InputSensor inputSensor(input);
                const auto& metaData = inputSensor.getSpec().getMetaData();
                if (metaData.find("parent") != metaData.end()) {
                    const auto* parentName = metaData.at("parent").get<const char*>();

                    for (auto input2 : { inputs... }) {
                        hub::sensor::InputSensor inputSensor2(input2);
                        const auto& parentSensorName = inputSensor2.getSpec().getSensorName();
                        if (parentSensorName == parentName) {
//                            std::cout << "parent name : " << parentName << std::endl;
                            hub::sensor::InputSensor inputSyncSensor(input, input2);
                            specAcqs.push_back(inputSyncSensor.getSpec());
                            inputSyncSensor.fillAllAcquisitions(specAcqs.back().m_acqs);
                        }
                    }
                }
            }

            return specAcqs;
        }

        template <class Input>
        std::vector<SpecAcqs> synchonizeInputs(std::vector<Input>& inputs)
        {
            std::vector<SpecAcqs> specAcqs;

            for (auto& input : inputs) {
                hub::sensor::InputSensor inputSensor(input);
                const auto& metaData = inputSensor.getSpec().getMetaData();
                if (metaData.find("parent") != metaData.end()) {
                    const auto* parentName = metaData.at("parent").get<const char*>();

                    for (auto& input2 : inputs) {
                        hub::sensor::InputSensor inputSensor2(input2);
                        const auto& parentSensorName = inputSensor2.getSpec().getSensorName();
                        if (parentSensorName == parentName) {
//                            std::cout << "parent name : " << parentName << std::endl;
                            hub::sensor::InputSensor inputSyncSensor(input, input2);
                            specAcqs.push_back(inputSyncSensor.getSpec());
                            inputSyncSensor.fillAllAcquisitions(specAcqs.back().m_acqs);
                        }
                    }
                }
            }

            return specAcqs;
        }

        std::vector<hub::input::InputFile> getInputFiles(const std::string & dir) {
            assert(std::filesystem::exists(dir));
            assert(std::filesystem::is_directory(dir));
            std::filesystem::path path(dir);

            //            std::vector<std::string> fileNames;
            std::vector<hub::input::InputFile> inputFiles;
            assert(std::filesystem::exists(path));
            assert(std::filesystem::is_directory(path));
//            std::cout << "path : " << path.string() << std::endl;

            for (const auto& filePath : std::filesystem::directory_iterator(path)) {
//                std::cout << "file : " << filePath << std::endl;
                //                if ( ! std::filesystem::exists( filePath ) )
                //                    continue;
                assert(std::filesystem::exists(filePath));
                                if (! std::filesystem::is_regular_file(filePath))
                                    continue;
                assert(std::filesystem::is_regular_file(filePath));

                //                hub::input::InputFile inputFile(filePath.string());
                //                inputFiles.push_back(std::move(inputFile));
                inputFiles.emplace_back(filePath.path().string());
            }

            return inputFiles;
        }

        void synchronizePath(const std::string& dir)
        {
            auto inputFiles = getInputFiles(dir);

            const auto syncDir = dir + "sync/";
            std::filesystem::create_directory(syncDir);

            const auto & specAcqs = synchonizeInputs(inputFiles);
//            std::cout << "spec acqs : " << specAcqs << std::endl;
            for (const auto& specAcq : specAcqs) {
                const auto & sensorSpec = specAcq.m_sensorSpec;
                const auto & acqs = specAcq.m_acqs;
//                std::cout << specAcq.m_sensorSpec << std::endl;
//                for (const auto& acq : specAcq.m_acqs) {
//                    std::cout << acq << std::endl;
//                }
                const std::string syncFile = syncDir + specAcq.m_sensorSpec.getSensorName() + ".hub";
                hub::sensor::OutputSensor outputSensor(sensorSpec, hub::output::OutputFile(hub::io::make_header(sensorSpec), syncFile));
                outputSensor.fill(acqs);
            }

            // auto inputSensors = getInputSensors( filePaths );
            //    auto inputSensor = getInputSensor( filePaths );

            // todo hub2
            // for ( const auto& pair : inputSensors ) {
            // const auto& sensorName = pair.first;
            // auto& inputSensor      = *pair.second;

            //            const auto& sensorSpec = inputSensor->getSpec();
            //            const auto& metaData = sensorSpec.getMetaData();
            //            const auto& sensorName = sensorSpec.getSensorName();

            // if ( metaData.find( "parent" ) != metaData.end() ) {
            // const char* parent = metaData.at( "parent" ).getConstCharPtr();
            // const char* parent = metaData.at( "parent" ).get<const char*>();

            // if ( inputSensors.find( parent ) != inputSensors.end() ) {

            // auto& parentInputSensor      = *inputSensors.at( parent );
            // const auto& parentSensorSpec = parentInputSensor.getSpec();
            // const auto& parentSensorName = parentSensorSpec.getSensorName();

            //            std::filesystem::create_directories(outputPath);
            // std::fstream recordFile( outputPath + sensorName + "_" + parentSensorName + ".txt",
            //            const std::string recordFile = outputPath + sensorName + ".txt";
            // std::ios::out | std::ios::binary | std::ios::trunc );
            // assert( recordFile.is_open() );

            //            std::vector<hub::sensor::Acquisition> syncAcqs;
            //            syncAcqs = inputSensor->getAllAcquisitions();

            // auto& input  = inputSensor.getInput();
            // auto& input2 = parentInputSensor.getInput();
            // int iAcq     = 0;
            // int maxAcq   = 1'000;
            // int minAcq   = 0;
            // while ( !input.isEnd() && !input2.isEnd() && iAcq < maxAcq ) {
            //     hub::sensor::Acquisition acq;
            //     input >> input2 >> acq;
            //     if ( iAcq >= minAcq ) syncAcqs.push_back( std::move( acq ) );
            //     std::cout << "[Loader] get sync acq " << iAcq << std::endl;
            //     ++iAcq;
            // }

            // hub::sensor::SensorSpec syncSensorSpec          = sensorSpec + parentSensorSpec;
            // hub::sensor::SensorSpec::MetaData& syncMetaData = syncSensorSpec.getMetaData();
            // syncMetaData.erase( "model" );
            // syncMetaData["nAcq"] = (int)syncAcqs.size();
            //    auto outputSensor = hub::make_outputSensor<hub::output::OutputFile>( sensorSpec, recordFile );
            // hub::sensor::OutputSensor outputSensor( sensorSpec,
            // hub::output::OutputFile{} );

            //    for ( const auto& syncAcq : syncAcqs ) {
            //        outputSensor << syncAcq;
            //    }
            // }
        }

    } // utils
} // namespace sensor
} // namespace hub
