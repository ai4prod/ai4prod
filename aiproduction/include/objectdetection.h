/*

GNU GPL V3 License

Copyright (c) 2020 Eric Tondelli. All rights reserved.

This file is part of Ai4prod.

Ai4prod is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Ai4prod is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Ai4prod.  If not, see <http://www.gnu.org/licenses/>

*/

#include "modelInterface.h"
#include "math.h"
#include "algorithm"

namespace ai4prod
{

    namespace objectDetection
    {

        class AIPRODUCTION_EXPORT Yolov3 : ai4prod::modelInterfaceObjectDetection
        {

        private:
            //INIT VARIABLE

            YAML::Node m_ymlConfig;
            std::string m_sModelTrPath;
            std::string m_sModelOnnxPath;
            std::string m_sEngineFp;
            std::string m_sEngineCache;

            MODE m_eMode;

            //neural network input dimension
            int m_iInput_h;
            int m_iInput_w;
            //original image width and height
            int m_iMrows;
            int m_iMcols;

            int m_iNumClasses;
            float m_fNmsThresh;
            float m_fDetectionThresh;

            //ONNX RUNTIME

            Ort::SessionOptions m_OrtSessionOptions;
            Ort::AllocatorWithDefaultOptions allocator;

            std::unique_ptr<Ort::Session> m_OrtSession;
            std::unique_ptr<Ort::Env> m_OrtEnv;

            //OnnxRuntime Input Model

            size_t num_input_nodes;
            std::vector<const char *> input_node_names;

            //OnnxRuntime Output Model

            size_t num_out_nodes;

            // onnx runtime data
            float *m_fpInputOnnxRuntime;
            float *m_fpOutOnnxRuntime[2];

            //Model Out
            std::vector<int64_t> m_viNumberOfBoundingBox;

            //Input dimension onnx model
            size_t m_InputTorchTensorSize;

            //LIBTORCH
            torch::Tensor m_TInputTorchTensor;
            torch::Tensor m_TOutputTensor;

            //THREAD SAFE

            //handle initialization
            bool m_bInit;
            //used to call init only one time per instances
            bool m_bCheckInit;
            //used to verify if preprocess is called on the same run
            bool m_bCheckPre;
            //used to verify if run model is called on the same run
            bool m_bCheckRun;
            //used to verify id post process is called
            bool m_bCheckPost;

            //UTILS

            ai4prod::aiutils aut;

            //MESSAGE/ERROR HANDLING

            std::string m_sMessage;

            //FUNCTION

            //init

            void setOnnxRuntimeEnv();
            void setOnnxRuntimeModelInputOutput();
            bool checkParameterConfig(std::string modelPathOnnx, int input_h, int input_w, int numClasses, MODE t, std::string model_path);
            bool createYamlConfig(std::string modelPathOnnx, int input_h, int input_w, int numClasses, MODE t, std::string model_path);
            void setSession();
            
            std::vector<int> *m_setReturnClass;

            //Preprocessing
            //to preserve aspect ratio of image
            cv::Mat padding(cv::Mat &img, int width, int weight);

            //Accuracy

            //array with all detection accuracy
            Json::Value m_JsonRootArray;

            //use internally for detection accuracy
            cv::Rect get_RectMap(float bbox[4]);

        public:
            //string to save image id for accuracy detection
            std::string m_sAccurayImagePath;

            Yolov3();

            virtual ~Yolov3();

            bool init(std::string modelPathOnnx, int input_h, int input_w, int numClasses, MODE t, std::string model_path = NULL,std::vector<int> *returnClass = nullptr);

            void preprocessing(cv::Mat &Image);
            torch::Tensor postprocessing(std::string imagePathAccuracy="");
            void runmodel();

            void createAccuracyFile();
            //return width of input image
            int getWidth()
            {

                return m_iInput_w;
            }

            //return height of input image
            int getHeight()
            {

                return m_iInput_h;
            }

            std::string getMessage()
            {

                return m_sMessage;
            }

            //call from outside class to draw rectangle given image
            cv::Rect get_rect(cv::Mat &img, float bbox[4]);

            //void nms(vector<float> &detection,float nsm_thres=0.9);

            float iou(float lbox[4], float rbox[4]);
        };

        class AIPRODUCTION_EXPORT Yolov4 : ai4prod::modelInterfaceObjectDetection
        {
        private:

            YAML::Node m_ymlConfig;
            std::string m_sModelTrPath;
            std::string m_sModelOnnxPath;
            std::string m_sEngineFp;
            std::string m_sEngineCache;

            MODE m_eMode;

            //neural network input dimension
            int m_iInput_h;
            int m_iInput_w;
            int m_iPaddingDimension;
            //original image width and height
            int m_iMrows;
            int m_iMcols;

            int m_iNumClasses;
            float m_fNmsThresh;
            float m_fDetectionThresh;

            //ONNX RUNTIME

            Ort::SessionOptions m_OrtSessionOptions;
            Ort::AllocatorWithDefaultOptions allocator;

            std::unique_ptr<Ort::Session> m_OrtSession;
            std::unique_ptr<Ort::Env> m_OrtEnv;

            //OnnxRuntime Input Model

            size_t num_input_nodes;
            std::vector<const char *> input_node_names;

            //OnnxRuntime Output Model

            size_t num_out_nodes;

            // onnx runtime data
            float *m_fpInputOnnxRuntime;
            float *m_fpOutOnnxRuntime[2];

            //Model Out
            std::vector<int64_t> m_viNumberOfBoundingBox;

            //Input dimension onnx model
            size_t m_InputTorchTensorSize;

            //LIBTORCH
            torch::Tensor m_TInputTorchTensor;
            torch::Tensor m_TOutputTensor;

            //THREAD SAFE

            //handle initialization
            bool m_bInit;
            //used to call init only one time per instances
            bool m_bCheckInit;
            //used to verify if preprocess is called on the same run
            bool m_bCheckPre;
            //used to verify if run model is called on the same run
            bool m_bCheckRun;
            //used to verify id post process is called
            bool m_bCheckPost;

            //UTILS

            ai4prod::aiutils aut;

            //MESSAGE/ERROR HANDLING

            std::string m_sMessage;

            //FUNCTION

            //init

            void setOnnxRuntimeEnv();
            void setOnnxRuntimeModelInputOutput();
            bool checkParameterConfig(std::string modelPathOnnx, int input_h, int input_w, int numClasses, MODE t, std::string model_path);
            bool createYamlConfig(std::string modelPathOnnx, int input_h, int input_w, int numClasses, MODE t, std::string model_path);
           
            void setSession();

            //set to exclude some class from output

            std::vector<int> *m_setReturnClass;

            //preprocessing
            cv::Mat padding(cv::Mat &img, int width, int weight);

            torch::Tensor cpuNms(torch::Tensor boxes, torch::Tensor confs,float nmsThresh=0.5);
            
            //MAP

            cv::Rect getRectMap(float bbox[4]);
            
            //array with all detection accuracy
            Json::Value m_JsonRootArray;


        public:
            //string to save image id for accuracy detection
            std::string m_sAccurayImagePath;
            
            Yolov4();

            virtual ~Yolov4();

            bool init(std::string modelPathOnnx, int input_h, int input_w, int numClasses, MODE t, std::string model_path = NULL, std::vector<int> *returnClass = nullptr);

            void preprocessing(cv::Mat &Image);

            void runmodel();
            cv::Rect getRect(cv::Mat &image,  float bbox[4] );

            torch::Tensor postprocessing(std::string imagePathAccuracy="");

            void createAccuracyFile();

            
        };

    } // namespace objectDetection

} //namespace ai4prod
