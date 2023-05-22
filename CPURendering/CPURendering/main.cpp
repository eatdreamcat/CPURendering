// Cpu Rendering.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
#include "GPUDevice.h"
#include "Stats.h"

#ifndef RUN_TIME
#define RUN_TIME 1
#endif // !RUN_TIME

#if !(RUN_TIME)
#include "TestCode.hpp"
#endif

void CreateVBO(RootSignature& rootSignature, FbxMesh* fbxMesh, int level, Stats& stats) {

    if (fbxMesh->RemoveBadPolygons() < 0) return;

    if (fbxMesh->GetPolygonCount() < 0) return;

    const int verticesCount         { fbxMesh->GetControlPointsCount() };
    FbxVector4* controlPoints       { fbxMesh->GetControlPoints() };
    const int indicesCount          { fbxMesh->GetPolygonVertexCount() };
    int* indices                    { fbxMesh->GetPolygonVertices() };
    
    assert(verticesCount > 0 && controlPoints && indicesCount > 0 && indices);

    if (!(verticesCount > 0 && controlPoints && indicesCount > 0 && indices)) return;

    for (int i{ 0 }; i < verticesCount; ++i) {
        rootSignature.vob.vertice.push_back(std::clamp((float)controlPoints[i].mData[0], -1.f, 1.f));
        rootSignature.vob.vertice.push_back(std::clamp((float)controlPoints[i].mData[1], -1.f, 1.f));
        rootSignature.vob.vertice.push_back(std::clamp((float)controlPoints[i].mData[2], -1.f, 1.f));
    }

    for (int i{ 0 }; i < indicesCount; ++i) {
        rootSignature.vob.indices.push_back(indices[i]);
    }

    stats.vertices += rootSignature.vob.vertice.size();
    stats.triangles += rootSignature.vob.indices.size() / 3;
}


void TravelScene(GPUDevice& gpu, FbxNode* node, Stats& stats, int level = 0) {

    if (node) {


        if (node->GetMesh()) {

            std::vector<float> vertice;
                std::vector<unsigned int> indices;

            VertexBuffer vbo{
                    vertice,
                    indices,
                    VertexLayout :: Vertex
            };

            RootSignature rootSignature{ 
                vbo,
                Primitive::Line
            };

            CreateVBO(rootSignature, node->GetMesh(), level, stats);

            gpu.Draw(rootSignature, stats);

        }

        for (int i = 0; i < node->GetChildCount(); i++)
        {
            TravelScene(gpu, node->GetChild(i), stats, level + 1);
        }
    }
}

FbxNode* LoadAssets(const char* lFilename, FbxManager* lSdkManager) {

    //// Change the following filename to a suitable filename value.
    //const char* lFilename = "/Iron_Man.FBX";
    // Initialize the SDK manager. This object handles memory management.
   /* FbxManager* lSdkManager = FbxManager::Create();*/
    // Create the IO settings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize((current_working_directory() + lFilename).c_str(), -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);
    // The file is imported, so get rid of the importer.
    lImporter->Destroy();



    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    return lScene->GetRootNode();
}

int main()
{

#if RUN_TIME
    {

        //// Change the following filename to a suitable filename value.
        const char* lFilename = "/Iron_Man.FBX";
        // Initialize the SDK manager. This object handles memory management.
        FbxManager* lSdkManager = FbxManager::Create();

        FbxNode* root = LoadAssets(lFilename, lSdkManager);

        Stats stats{
            0.0f,  // fps
            0,  // dc,
            0,// vertices
            0// triangles
        };

        using namespace std;

        auto width = 1280;
        auto height = 720;

        GPUDevice gpu;

        gpu.InitWindow("Cpu Rendering");
        gpu.CreateFrameBuffers(height, width);

        auto keyCode = 0;



        while (keyCode != 27) {

            // before draw
            stats.DrawCalls = 0;
            stats.triangles = 0;
            stats.vertices = 0;

            gpu.BeforeRendering();
            gpu.Clear();

            // travel scene and draw
            TravelScene(gpu, root, stats);

            // swap back front buffer
            gpu.Present(stats);

            
            keyCode = pollKey();
        
        }

      /*  gpu.BeforeRendering();
        gpu.Clear();

        gpu.OnRendering();
        gpu.Present();*/

        waitKey();
        destroyAllWindows();

        lSdkManager->Destroy();
       
        /* auto p = new int*[2];
         p[0] = new int;
         p[1] = new int;
         delete p[0];
         delete p[1];
         delete []p;*/
    }
#else
    {
        FBX_Test();

    }
#endif

    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
