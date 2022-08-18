#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <File.h>
#include <filesystem>
#include <fstream>
#include <stream.h>
#include <string>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    m_imageManipulator = new FormImageManipulator(this);
    //    m_imageManipulator->setMinimumWidth(640);
    //    m_imageManipulator->setMinimumHeight(1000);

    //    FormIm
    //    ui->mdiArea->addSubWindow(m_imageManipulator);

    {
//        std::string filepath = PROJECT_DIR "data/records/2022-05-27_10-16-00/webcam.txt";
        std::string filepath = PROJECT_DIR "data/records/2022-05-23_13-32-34 (carotide)/ULA-OP 256.txt";
        assert(std::filesystem::exists(filepath));
        std::fstream file(filepath, std::ios::binary | std::ios::in);

        InputStream inputStream(File(std::move(file)));
        const auto & header = inputStream.getHeader();
        const auto& acqs = inputStream.getAllAcquisition();
        const auto& acq = acqs[4];
        const auto& dims = header.getDims();

        //        ui->widgetStreamView->init(640, 480);
        m_streamView2D = new WidgetStreamView2D(this);
        //        m_streamView2D->setMinimumWidth(640);
        //        m_streamView2D->setMinimumHeight(480);
        m_streamView2D->init(dims.at(0), dims.at(1));
        m_streamView2D->setData((unsigned char*)acq.mData, header.getAcquisitionSize(), header.getDims(), header.getFormat());

        ui->mdiArea->addSubWindow(m_streamView2D);
    }

    {
        std::string filepath = PROJECT_DIR "data/records/2022-05-23_13-32-34 (carotide)/ULA-OP 256.txt";
        assert(std::filesystem::exists(filepath));
        std::fstream file(filepath, std::ios::binary | std::ios::in);

        InputStream inputStream(File(std::move(file)));
        const auto & header = inputStream.getHeader();
        const auto& acqs = inputStream.getAllAcquisition();
        const auto& acq = acqs[163];
        const auto& dims = header.getDims();

        //        ui->widgetStreamView->init(dims.at(0), dims.at(1));
        m_streamView2D_2 = new WidgetStreamView2D(this);
        m_streamView2D_2->init(512, 192, 35.0, 50.0);
//        m_streamView2D_2->init(512, 192);
        m_streamView2D_2->setData((unsigned char*)acq.mData, header.getAcquisitionSize(), header.getDims(), header.getFormat());
        ui->mdiArea->addSubWindow(m_streamView2D_2);

//    }


//    {
        m_imageManipulator = new FormImageManipulator(this);
        m_imageManipulator->setMinimumWidth(400);
        m_imageManipulator->setMinimumHeight(900);
        auto & widgetStreamView = m_imageManipulator->getWidgetStreamView();
        widgetStreamView.init(512, 192, 35.0, 50.0);
//        widgetStreamView.init(512, 192);
        widgetStreamView.setData((unsigned char*)acq.mData, header.getAcquisitionSize(), header.getDims(), header.getFormat());

    //    FormIm
        ui->mdiArea->addSubWindow(m_imageManipulator);

    }

}

MainWindow::~MainWindow()
{
    delete m_streamView2D;
    delete m_streamView2D_2;
    delete ui;
}
