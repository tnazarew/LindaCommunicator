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

            lindaInstanceStruct.out = getStringFromInputStream(p.getInputStream());
            lindaInstanceStruct.error = getStringFromInputStream(p.getErrorStream());

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private String getStringFromInputStream(InputStream is) {
        try {
            String str = new String();

            int data = is.read();
            while (data != -1) {
                str += (char) data;
                data = is.read();
            }
            return str;
        } catch (IOException ex)
        {
            return null;
        }
    }
}
