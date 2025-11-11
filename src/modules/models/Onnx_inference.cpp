#include <Onnx_inference.h>

Inference::Inference() {}

Inference::Inference(const std::string& onnxModelPath, const cv::Size& modelInputShape, const std::string& classesTxtFile, const bool& runWithCuda)
{
    modelPath = onnxModelPath;
    modelShape = modelInputShape;
    classesPath = classesTxtFile;
    cudaEnabled = runWithCuda;

    loadOnnxNetwork();
}

std::vector<Detection> Inference::runInference(const cv::Mat& input)
{
    cv::Mat modelInput = input;
    if (letterBoxForSquare && modelShape.width == modelShape.height)
        modelInput = formatToSquare(modelInput);

    cv::Mat blob;
    cv::dnn::blobFromImage(modelInput, blob, 1.0 / 255.0, modelShape, cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    int rows = outputs[0].size[1];
    int dimensions = outputs[0].size[2];
    float* data = (float*)outputs[0].data;

    float x_factor = modelInput.cols / modelShape.width;
    float y_factor = modelInput.rows / modelShape.height;

    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < rows; ++i) {
        float confidence = data[4];

        if (confidence >= modelConfidenceThreshold)
        {
            float* classes_scores = data + 5;

            cv::Mat scores(1, classes.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;

            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

            if (max_class_score > modelScoreThreshold)
            {
                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];

                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);

                int width = int(w * x_factor);
                int height = int(h * y_factor);

                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
        data += dimensions;
    }

    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, modelScoreThreshold, modelNMSThreshold, nms_result);

    std::vector<Detection> detections{};
    for (unsigned long i = 0; i < nms_result.size(); ++i)
    {
        int idx = nms_result[i];

        Detection result;
        result.class_id = class_ids[idx];
        result.confidence = confidences[idx];

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(100, 255);
        // result.color = cv::Scalar(dis(gen),
        //     dis(gen),
        //     dis(gen));
        result.color = cv::Scalar(34, 34, 34);

        result.className = classes[result.class_id];
        result.box = boxes[idx];

        detections.push_back(result);
    }

    return detections;
}

void Inference::loadClassesFromFile()
{
    std::ifstream inputFile(classesPath);
    if (inputFile.is_open())
    {
        std::string classLine;
        while (std::getline(inputFile, classLine))
            classes.push_back(classLine);
        inputFile.close();
    }
}

void Inference::loadOnnxNetwork()
{
    net = cv::dnn::readNetFromONNX(modelPath);
    if (cudaEnabled)
    {
        std::cout << "Yolo model is Running on CUDA" << '\n';
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    }
    else
    {
        std::cout << "Yolo model is Running on CPU" << '\n';
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    }
}

cv::Mat Inference::formatToSquare(const cv::Mat& source)
{
    cv::Mat cpy;
    cv::cvtColor(source, cpy, cv::COLOR_RGBA2BGR);
    int col = source.cols;
    int row = source.rows;
    int _max = MAX(col, row);
    cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);

    cpy.copyTo(result(cv::Rect(0, 0, col, row)));
    return result;
}

cv::Mat Inference::draw_box(const cv::Mat& img, const std::vector<Detection>& detected_objects) const {
    int detections = static_cast<int>(detected_objects.size());
    cv::Mat result;
    cv::cvtColor(img, result, cv::COLOR_RGBA2BGR);
    
    for (int i = 0; i < detections; ++i) // Generating bounding boxes for objects.
    {
        Detection detection = detected_objects[i];

        cv::Rect box = detection.box;
        cv::Scalar color = detection.color;

        cv::rectangle(result, box, color, 1);

        std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
        cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
        cv::Rect textBox(box.x, box.y - 27, textSize.width - 90, textSize.height + 5);

        cv::rectangle(result, textBox, color, cv::FILLED);
        cv::putText(result, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 1, 0);
    }

    cv::resize(result, result, cv::Size(178, 218));
    cv::cvtColor(result, result, cv::COLOR_BGR2RGBA);

    return result;
}