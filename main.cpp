#include <QApplication>       // Подключаем QApplication для создания графического приложения.
#include <QWidget>            // Подключаем QWidget — базовый класс для всех виджетов.
#include <QVBoxLayout>        // Подключаем QVBoxLayout для организации вертикального расположения виджетов.
#include <QLabel>             // Подключаем QLabel для отображения текста.
#include <QTcpSocket>         // Подключаем QTcpSocket для работы с TCP-соединениями.
#include <QDataStream>        // Подключаем QDataStream для сериализации и десериализации данных.

class MyClientWidget : public QWidget {
    Q_OBJECT   // Макрос Q_OBJECT необходим для работы сигналов и слотов.
public:
    MyClientWidget(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        // Создаем QLabel для отображения информации и задаем начальный текст.
        label = new QLabel("Waiting for data from the server...", this);
        layout->addWidget(label);
        // Создаем объект QTcpSocket для установления TCP-соединения с сервером.
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::readyRead, this, &MyClientWidget::readData);
        connect(socket, &QTcpSocket::disconnected, this, &MyClientWidget::connectionLost);

        // Инициируем подключение к серверу по адресу 127.0.0.1 и порту 1234.
        socket->connectToHost("127.0.0.1", 1234);
    }

private slots:
    void readData() {
        // Создаем QDataStream для чтения данных из сокета.
        QDataStream in(socket);
        qint32 clientCount;
        in >> clientCount;
        // Обновляем текст QLabel, выводя количество подключенных клиентов.
        label->setText(QString("Connected clients: %1").arg(clientCount));
    }
    void connectionLost() {
        label->setText("Connection lost");
    }

private:
    QLabel *label;
    QTcpSocket *socket;
};

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MyClientWidget widget;
    widget.setWindowTitle("TCP Client");
    widget.resize(300, 100);
    widget.show();
    return a.exec();
}

#include "main.moc"
