module com.mycompany.javachatbot {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;

    opens com.mycompany.javachatbot to javafx.fxml;
    exports com.mycompany.javachatbot;
}
