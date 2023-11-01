#include "STEPControl_Reader.hxx"
#include "StlAPI_Writer.hxx"
#include "TopoDS_Shape.hxx"
#include "Standard_ConstructionError.hxx"
#include "BRepMesh_IncrementalMesh.hxx"

#include <iostream>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <VTKWriter.h>
#include <StlAPI_Writer.hxx>

// VTK
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <BRepMesh_IncrementalMesh.hxx>
#include <vtkAutoInit.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkTransformFilter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#include <iostream>
#include <BRep_Builder.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESData_IGESModel.hxx>
#include <StlAPI_Writer.hxx>

#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <StlAPI_Writer.hxx>
#include <vtkCubeSource.h>

#include <thread>
#include <chrono>


VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int step2stl(char* in, char* out) {

    // Read from STEP
    STEPControl_Reader reader;
    IFSelect_ReturnStatus stat = reader.ReadFile(in);

    Standard_Integer NbRoots = reader.NbRootsForTransfer();
    Standard_Integer NbTrans = reader.TransferRoots();
    TopoDS_Shape Original_Solid = reader.OneShape();

    // Write to STL
    StlAPI_Writer stlWriter = StlAPI_Writer();
    //stlWriter.SetCoefficient(0.0001);
    stlWriter.ASCIIMode() = Standard_False;

    BRepMesh_IncrementalMesh Mesh(Original_Solid, 0.01);

    Mesh.Perform();

    stlWriter.Write(Original_Solid, out);

    return 0;
}

// Hàm tạo chuỗi ngẫu nhiên
std::string generateRandomString(int length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGH"
        "abcdefgh";

    std::string randomString;
    randomString.reserve(length);

    static std::mt19937 generator(time(0));
    std::uniform_int_distribution<int> distribution(0, sizeof(alphanum) - 2);

    for (int i = 0; i < length; ++i) {
        randomString += alphanum[distribution(generator)];
    }

    return randomString;
}

enum Position
{
    TOP,
    LEFT,
    RIGHT,
    BACK,
    BOTTOM,
    FRONT,
    DEFAULT
};

void writeResult(Position po, std::string id) {
    switch (po)
    {
    case TOP:
        std::cout << "TOP:" << id << std::endl;
        break;
    case LEFT:
        std::cout << "LEFT:" << id << std::endl;
        break;
    case RIGHT:
        std::cout << "RIGHT:" << id << std::endl;
        break;
    case BACK:
        std::cout << "BACK:" << id << std::endl;
        break;
    case BOTTOM:
        std::cout << "BOTTOM:" << id << std::endl;
        break;
    case FRONT:
        std::cout << "FRONT:" << id << std::endl;
        break;
    case DEFAULT:
        std::cout << "DEFAULT:" << id << std::endl;
        break;
    default:
        break;
    }
}
void screenshot(vtkSmartPointer<vtkRenderWindow> renderWindow, vtkSmartPointer<vtkRenderer> renderer, Position po) {
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(renderWindow);
    renderer->ResetCamera();
    renderWindow->Render();
    // Specify the output format (e.g., PNG)
    windowToImageFilter->SetInputBufferTypeToRGBA();
    windowToImageFilter->ReadFrontBufferOn();
    windowToImageFilter->Update();
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    std::string id = generateRandomString(36);
    std::string name = "D:\\" + id + ".png";
    writeResult(po,id);
    writer->SetFileName(&name[0]);
    writer->SetInputData(windowToImageFilter->GetOutput());

    writer->Write();
}


void Rotate(vtkSmartPointer<vtkActor> actor, vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkRenderWindow> renderWindow) {
    

    actor->RotateWXYZ(-90, 1, 0, 0); //-90, 1, 0, 0
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    screenshot(renderWindow, renderer, Position::TOP);
    actor->RotateWXYZ(90, 1, 0, 0);


    actor->RotateWXYZ(90, 0, 1, 0);//90, 0, 1, 0
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
   screenshot(renderWindow, renderer, Position::LEFT);
    actor->RotateWXYZ(-90, 0, 1, 0);


    actor->RotateWXYZ(-90, 0, 1, 0); //-90, 0, 1, 0
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    screenshot(renderWindow, renderer, Position::RIGHT);
    actor->RotateWXYZ(90, 0, 1, 0);


    actor->RotateWXYZ(0, 0, 0, 0); //0, 0, 0, 0
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    screenshot(renderWindow, renderer, Position::BACK);
   

    actor->RotateWXYZ(90, 1, 0, 0);//90, 1, 0, 0
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    screenshot(renderWindow, renderer, Position::BOTTOM);
    actor->RotateWXYZ(-90, 1, 0, 0);

    actor->RotateWXYZ(180, 1, 0, 0);//180, 1, 0, 0
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    screenshot(renderWindow, renderer, Position::FRONT);
    actor->RotateWXYZ(-180, 1, 0, 0);

    actor->RotateWXYZ(45, 10, 10, 3.5); //45, 10, 10, 3.5
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    screenshot(renderWindow, renderer, Position::DEFAULT);
  
}

int main()
{
    std::string t1 = "D:\\hinh11.STEP";
    std::string t2 = "D:\\output.stl";
    step2stl(&t1[0], &t2[0]);
       

        // Tạo một cube source
        vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();

        // Thiết lập kích thước của box bằng cách đặt các giá trị của 3 hệ số scale
        cubeSource->SetXLength(15);  // Chiều dài theo trục x
        cubeSource->SetYLength(15);  // Chiều dài theo trục y
        cubeSource->SetZLength(15);  // Chiều dài theo trục z

        // Initialize VTK
        vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
        stlReader->SetFileName(&t2[0]);

        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(stlReader->GetOutputPort());

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);

        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renderWindowInteractor->SetRenderWindow(renderWindow);

        renderer->AddActor(actor);

        // Create an interactor style for camera interaction
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        renderWindowInteractor->SetInteractorStyle(style);

        //set size windows box
        renderWindow->SetSize(500, 500);

        Rotate(actor, renderer, renderWindow);

        // Re-render the scene with the rotated actor

        //renderWindowInteractor->Start();

        std::cout << "Conversion completed successfully." << std::endl;
   
    return 0;
}