package pl.edu.pw.testlinda;

import java.io.IOException;
import java.io.InputStream;

public class LindaRunnable implements Runnable {

    LindaInstanceStruct lindaInstanceStruct;
    LindaRunnable(LindaInstanceStruct lindaInstanceStruct)
    {
        this.lindaInstanceStruct = lindaInstanceStruct;
    }
    @Override
    public void run() {
        Process p;
        try {
            String s = new String();
            s += Constants.path;
            s += " " + lindaInstanceStruct.lindaTypeEnum.toString();
            s += " " + lindaInstanceStruct.pattern;

            p = Runtime.getRuntime().exec(s);

            lindaInstanceStruct.process = p;
            lindaInstanceStruct.ret = p.waitFor();

            //255 return code is equal -1 in C/C++
            if (lindaInstanceStruct.ret == 255) {
                try {
                    String error = new String();

                    InputStream is = p.getErrorStream();
                    int data = is.read();
                    while (data != -1) {
                        error += (char) data;
                        data = is.read();
                    }
                    lindaInstanceStruct.error = error;
                } catch (IOException ex)
                {}
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
