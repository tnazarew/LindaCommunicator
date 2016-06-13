package pl.edu.pw.testlinda;

import java.util.ArrayList;

public abstract class Test {

    public final String testName = this.getClass().getSimpleName();

    public String description;

    public abstract boolean run ();

    protected ArrayList<LindaInstanceStruct> startNInstances (int n, LindaTypeEnum lindaTypeEnum, String pattern)
    {
        ArrayList<LindaInstanceStruct> outputList = new ArrayList<>();
        for (int i = 0; i < n; i++)
        {
            LindaInstanceStruct lindaInstanceStruct =
                    new LindaInstanceStruct(lindaTypeEnum, pattern);

            outputList.add(lindaInstanceStruct);
            LindaRunnable lindaRunnable = new LindaRunnable(lindaInstanceStruct);
            new Thread(lindaRunnable).start();
        }
        return outputList;
    }

    protected Result checkResults(ArrayList<LindaInstanceStruct> list)
    {
        Result result = new Result();
        for (LindaInstanceStruct lindaInstanceStruct : list)
        {
            if (lindaInstanceStruct.ret != null && lindaInstanceStruct.ret == 0) {
                result.finished++;
            }
            else if (lindaInstanceStruct.ret != null && lindaInstanceStruct.ret == 255) {
                result.error++;
                System.out.println("Error: " + lindaInstanceStruct.lindaTypeEnum + " " + lindaInstanceStruct.pattern);
                if (lindaInstanceStruct.error != null) {
                    System.out.println("Message: " + lindaInstanceStruct.error);
                }
            }
            else {
                result.waiting++;
                //kill processess that are still running
                lindaInstanceStruct.process.destroyForcibly();
            }
        }
        return result;
    }

    public class Result {

        public int finished;
        public int waiting;
        public int error;

        Result ()
        {
            finished = 0;
            waiting = 0;
            error = 0;
        }

        public String getString()
        {
            String s = "Finished " + finished + ", Waiting: " + waiting + ", Error: " + error;
            return s;
        }
    }
}
