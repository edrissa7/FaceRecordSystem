#include "menu.h"
#include "ui_menu.h"
#include "login.h" //to include database connection stuff

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    // QObject::connect(ui->dateEdit, &Menu::dateChanged,ui->spinBox,&Menu::UpdateAge);
    ui->setupUi(this);
    this->setCentralWidget(ui->tabWidget);
    //add sqlite database and to myDb handler
   // myDb = QSqlDatabase::addDatabase("QSQLITE");
   // QString rd = QString::fromLocal8Bit(returnName.c_str());

}

Menu::~Menu()
{
    //destructor to delete ui from heap
    delete ui;
}


void Menu::on_button_StartDetect_clicked()
{
if(ui->checkBox_viewType->isChecked()){
 faceRecognizer::startRecognizer();
}else{
    QString myDialog = QFileDialog::getOpenFileName();
    if(!myDialog.isEmpty()){
    std::string myImage = myDialog.toUtf8().constData();
        faceRecognizer::startRecognizer(myImage);
    }else{
        QMessageBox::critical(this,"file error","No file choosen/check file");
    }
}
 //QString rd = QString::fromLocal8Bit(returnName.c_str());
// ui->label_datapull->setText(rd);
}

void Menu::on_button_AddUser_clicked()
{

    //check whether to load image or video
   /* if(!ui->checkBox_mode->isChecked()){
        QString myDialog = QFileDialog::getOpenFileName();
        if(!myDialog.isEmpty()){//check whether file is not empty
        std::string myImage = myDialog.toUtf8().constData();
        cv::Mat image;
        image = cv::imread(myImage,cv::IMREAD_COLOR);
        cv::Scalar color(255,0,0);
        cv::Point p1(200,200);
        cv::Point p2;
        p2.x = 100;
        p2.y = 60;
        cv::rectangle(image,p1,p2,color,1);
        cv::namedWindow("Image detection",cv::WINDOW_AUTOSIZE);
        cv::imshow("IMage detection",image);
        cv::waitKey(0);
        cv::destroyAllWindows();
        }else{
            QMessageBox::information(this,"message","no file selected");
            return;
        }//end check file empty
    }else{*/
       runDetector();
        // }

}

void Menu::UpdateAge(QDate date){

        emit dateChanged(date);

}


void Menu::runDetector(){
string cascadeName, nestedCascadeName;



        // VideoCapture class for playing video for which faces to be detected
        VideoCapture capture;
        Mat frame, image;

     Rect myroi;//new region of interest cropper

                // PreDefined trained XML classifiers with facial features
        CascadeClassifier cascade, nestedCascade;
        double scale=1;
         int count = 0;//image counter
        // Load classifiers from "opencv/data/haarcascades" directory

        // Change path before execution
        cascade.load( "C:/Users/user/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml" ) ;


        capture.open(0);//am startinf the video using webcam(0)
        if( !capture.isOpened() )//checking whether the capture is opened
        {
            cout<<"Could not Open Camera";
              return;
          }
            // Capture frames from video and detect faces
            cout << "Face Detection Started...." << endl;
            while(1)
            {
                ui->lineEdit_firstName->setText(window.personName);
                capture >> frame;
                if( frame.empty() )
                    break;
                Mat frame1 = frame.clone();
               this->detectAndDraw( frame1, cascade,scale,myroi);
                char c = waitKey(10);
                cvtColor(frame,image,COLOR_BGR2GRAY);
                // Press q to exit from window
                if(c == 'q'){
                    cout<<"face detection ended"<<endl;
                    cv::destroyAllWindows();
                    break;

                }else if(c == 't'){
                    QString s = QString::number(count);
                    std::string str = s.toUtf8().constData();//convert Qstring to regular string
                    crop = image(myroi); //crop image to be saved

                    window.myCrop = crop;
                    //------------------
                    QImage image1= QImage((uchar*)  window.myCrop.data,  window.myCrop.cols,  window.myCrop.rows,  window.myCrop.step, QImage::Format_RGB888);
                            //show Qimage using QLabel
                    window.imageLabel.setPixmap(QPixmap::fromImage(image1));
                    //------------------
                    window.setModal(true); //set ImageSave window
                    window.exec();//Open the imagesave Dialog Window
                     count++;
                   }
                }

            }




//detect and draw rectangle
    void Menu::detectAndDraw( Mat& img, CascadeClassifier& cascade,double scale,Rect& roi)
    {
        vector<Rect> faces;
        Mat gray, smallImg;

        cvtColor( img, gray, COLOR_BGR2GRAY ); // Convert to Gray Scale
        double fx = 1 / scale;

        // Resize the Grayscale Image
        cv::resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
        //It is a method that improves the contrast in an image, in order to stretch out the intensity range
        equalizeHist( smallImg, smallImg);

        //Detect faces of different sizes using cascade classifier
        cascade.detectMultiScale( smallImg, faces, 1.1,
                                2, 0|CASCADE_SCALE_IMAGE,  Size(smallImg.rows / 5, smallImg.rows / 5),
                                  Size(smallImg.rows * 2 / 3, smallImg.rows * 2 / 3));

         foundface = true;
        //region of interest code

        for ( size_t i = 0; i < faces.size(); i++ )
        {
            Rect r = faces[i];
            Scalar color = Scalar(255, 0, 0); // Color for Drawing rectangle

            //draw the rectangle on the face
            cv::rectangle(img,Point(r.x,r.y),Point((r.x+r.width),(r.y+r.height)),color,2);

            if(foundface){//making sure there is a face before copying coordinates for cropping
            roi.x = faces[0].x+20;
            roi.y = faces[0].y+20;
            roi.width = faces[0].width-30;
            roi.height = faces[0].height-10;
   }

   }
        imshow( "Face Detection", img );
}


void Menu::on_checkBox_stateChanged(int arg1)
{


}

void Menu::on_checkBox_editData_clicked()
{
    if(ui->checkBox_editData->isChecked()){
   ui->textEdit_dataEdit->setEnabled(true);
    }else{
       ui->textEdit_dataEdit->setEnabled(false);
    }
}

void Menu::on_pushButton_loadData_clicked()
{

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly| QFile::Text)){
        QMessageBox::critical(this,"File error","Cannot open file:");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit_dataEdit->setText(text);
    file.close();
}

void Menu::on_pushButton_dataSave_clicked()
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly| QFile::Text)){
        QMessageBox::critical(this,"File error","Cannot save file:");
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit_dataEdit->toPlainText();
    out<< text;
    QMessageBox::information(this,"Success","File saved");
    file.close();

}

void Menu::on_pushButton_train_clicked()
{
    dataTrainer.runTrainer();
}

void Menu::on_pushButton_add_clicked()
{
    Login conn;

    QString dob = ui->dateEdit_dob->text();
    QString offense_date = ui->dateEdit_offDate->text();
    int rid = ui->spinBox_id->value();
    QString firstname,lastname,middlename,gender, address,offense_description,offense_location;
    firstname = ui->lineEdit_firstName->text();
    lastname = ui->lineEdit_lastname->text();
    middlename = ui->lineEdit_middlename->text();
    //gender = ui->r
    address = ui->lineEdit_address->text();
    offense_description = ui->textEdit_offDesc->toPlainText();
    offense_location = ui->lineEdit_offLoc->text();

    QString insertQuery = "INSERT INTO records(rid,firstname,middlename,lastname,gender,dob,address,offDesc,"
                          "offDate,offLoc)"
                          "VALUES(:rid,:firstname,:middlename,:lastname,:gender,:dob,"
                          ":address,:offDesc,:offDate,:offLoc)";

    QString insertQuery2 = "INSERT INTO pathRecords(pid,path,label)"
                   "VALUES(:rid,:path,:label)";

   if(!conn.connOpen()){
       qDebug()<<("Failed to open connection");
       return;
   }
   conn.connOpen();
    QSqlQuery query;
    query.prepare(insertQuery);
     query.bindValue(":rid",rid);
    query.bindValue(":firstname",firstname);
      query.bindValue(":lastname",middlename);
        query.bindValue(":lastname",lastname);
          query.bindValue(":gender",gender);
          query.bindValue(":dob",dob);
            query.bindValue(":offDesc",offense_description);
            query.bindValue(":offDate",offense_date);
            query.bindValue(":offLoc",offense_location);


    if(!query.exec()){
        QMessageBox::critical(this,"error",query.lastError().text());

    }else{
       QMessageBox::information(this,"Message","Data Inserted");
       ui->spinBox_id->setValue(0);
       ui->lineEdit_firstName->setText("");
       ui->lineEdit_middlename->setText("");
       ui->lineEdit_lastname->setText("");
       ui->lineEdit_address->setText("");
      ui->textEdit_offDesc->setText("");
       ui->lineEdit_offLoc->setText("");
       conn.connClose();
    }

   }


void Menu::on_pushButton_searchPerson_clicked()
{
    QString firstname = ui->lineEdit_searchPerson->text();
    if(firstname.isEmpty()){
        QMessageBox::critical(this,"Empty Field","please enter a name in the field");
        return;
    }
    Login conn;
    QSqlQueryModel *model = new QSqlQueryModel();
    conn.connOpen();
    QSqlQuery *query = new QSqlQuery(conn.mydb);
    query->prepare("select * from records where firstname = '"+firstname+"'");
    query->exec();
    model->setQuery(*query);

    ui->tableView->setModel(model);
    conn.connClose();
}

void Menu::on_tableView_clicked(const QModelIndex &index)
{
    {

        rowIndex = QString::number(index.row()+1);

       Login conn;
        if(!conn.connOpen()){
            qDebug()<<"failed to open database";
            return;
        }

     ui->tabWidget->setCurrentIndex(1);

        conn.connOpen();
        QSqlQuery qry;
        //qry.prepare("select rid,firstname,middlename,lastname,gender,password,DOB,address, from users where rowid=6");
        qry.prepare("select * from records where rowid='"+rowIndex+"'");
        if(qry.exec()){
           int count = 0;
            while(qry.next()){
                count++;
                ui->spinBox_id->setValue(qry.value(0).toInt());
                ui->lineEdit_firstName->setText(qry.value(1).toString());
                ui->lineEdit_middlename->setText(qry.value(2).toString());
                ui->lineEdit_lastname->setText(qry.value(3).toString());
                if(qry.value(4).toString() == "male"){
                    ui->radioButton_male->setChecked(true);
                }else if(qry.value(4).toString() == "female"){
                    ui->radioButton_female->setChecked(true);
                }
                ui->dateEdit_dob->setDate(qry.value(5).toDate());
                ui->lineEdit_address->setText(qry.value(6).toString());
               ui->textEdit_offDesc->setText(qry.value(7).toString());
                ui->dateEdit_offDate->setDate(qry.value(8).toDate());
                ui->lineEdit_offLoc->setText(qry.value(9).toString());
              // ui->lineEdit_offLoc->setText((qry.value(11).toInt()));
       // cout << qry.value(6).toString().toUtf8().constData()<<endl;
            }
            if(count > 0){
             //ui->label->setText("Username and password are correct");
             conn.connClose();
        // QString firstname,lastname,username,password,usertype,rowid;
    }
    }
    }
}

void Menu::on_pushButton_2_clicked()
{
    Login conn;
    QDate mydate;
    QString date = mydate.currentDate().toString();

    int id;
    QDate dob;
    QDate offDate;
    QString firstname,middlename,lastname,gender,address,offDesc,offLoc;

    if(ui->radioButton_male->isChecked()){
        gender = "male";
    }else if(ui->radioButton_female->isChecked()){
        gender = "female";
    }

    id = ui->spinBox_id->value();
    firstname = ui->lineEdit_firstName->text();
    middlename = ui->lineEdit_middlename->text();
    lastname = ui->lineEdit_lastname->text();
    dob = ui->dateEdit_dob->date();
    address = ui->lineEdit_address->text();
    offDesc = ui->textEdit_offDesc->toPlainText();
    offDate = ui->dateEdit_offDate->date();
    offLoc = ui->lineEdit_offLoc->text();

    /* QString insertQuery = "update users set firstname=:firstname,lastname = :lastname,"
                           "username=:username,password=:password,dateAdded= :dateAdded,user_type=:usertype"
                           "where rowid = '"+rowIndex+"'";*/

   QString insertQuery = "UPDATE records SET firstname=?,middlename=?,lastname = ?,"
                         "gender=?,DOB=?,address=?,offDesc=?,offDate=?,offLoc=?"
                         " where rid = ?";

    if(!conn.connOpen()){
        qDebug()<<("Failed to open connection");
        return;
    }
    conn.connOpen();
     QSqlQuery query;
     query.prepare(insertQuery);
     query.addBindValue(firstname);
     query.addBindValue(middlename);
     query.addBindValue(lastname);
      query.addBindValue(gender);
     query.addBindValue(dob);
     query.addBindValue(address);
      query.addBindValue(offDesc);
       query.addBindValue(offDate);
        query.addBindValue(offLoc);
     query.addBindValue(id);
    /* query.prepare(insertQuery);
     query.bindValue(":firstname",firstname);
       query.bindValue(":lastname",lastname);
         query.bindValue(":username",username);
           query.bindValue(":password",password);
           query.bindValue(":dateAdded",date);
             query.bindValue(":usertype",usertype);*/


     if(!query.exec()){
         QMessageBox::critical(this,"error",query.lastError().text());

     }else{
        QMessageBox::information(this,"Message","Row updated");
        ui->spinBox_id->setValue(0);
        ui->lineEdit_firstName->setText("");
        ui->lineEdit_middlename->setText("");
        ui->lineEdit_lastname->setText("");
        ui->lineEdit_address->setText("");
       ui->textEdit_offDesc->setText("");
        ui->lineEdit_offLoc->setText("");
        conn.connClose();
      }
}


void Menu::on_pushButton_3_clicked()
{
    Login conn;
    int id = ui->spinBox_id->value();
    QString str = QString::number(id);


    if(!conn.connOpen()){
        qDebug()<<"failed to open database";
        return;
    }

    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("delete from users where id='"+str+"'");
    if(qry.exec()){
       QMessageBox::information(this,"Success!","Delete Successful");
       ui->spinBox_id->setValue(0);
       ui->lineEdit_firstName->setText("");
       ui->lineEdit_middlename->setText("");
       ui->lineEdit_lastname->setText("");
       ui->lineEdit_address->setText("");
      ui->textEdit_offDesc->setText("");
       ui->lineEdit_offLoc->setText("");
       conn.connClose();

      }
}

