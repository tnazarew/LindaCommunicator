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
            outputList.add(startInstance(lindaTypeEnum, pattern));

        }
        return outputList;
    }

    protected LindaInstanceStruct startInstance(LindaTypeEnum lindaTypeEnum, String pattern)
    {
        LindaInstanceStruct lindaInstanceStruct =
                new LindaInstanceStruct(lindaTypeEnum, pattern);

        new Thread(new LindaRunnable(lindaInstanceStruct)).start();
        return lindaInstanceStruct;
    }

    protected Result checkResultsAndClean(ArrayList<LindaInstanceStruct> list)
    {
        Result result = new Result();
        for (LindaInstanceStruct lindaInstanceStruct : list)
        {
            if (lindaInstanceStruct.ret != null && lindaInstanceStruct.ret == 0) {
                result.finished++;
            }
            else if (lindaInstanceStruct.ret != null && lindaInstanceStruct.ret == 255) {
                result.error++;
            }
            else {
                result.waiting++;
                //kill processess that are still running
                lindaInstanceStruct.process.destroyForcibly();
            }
            printErrorStream(lindaInstanceStruct);
        }
        cleanTmpFiles();
        return result;
    }

    protected Result checkResultsAndClean(ArrayList<LindaInstanceStruct> list, String expectedOutput)
    {
        boolean isOutputCorrect = true;
        Result result = new Result();
        for (LindaInstanceStruct lindaInstanceStruct : list)
        {
            if (lindaInstanceStruct.ret != null && lindaInstanceStruct.ret == 0) {
                result.finished++;
                if (!lindaInstanceStruct.out.contains(expectedOutput))
                    isOutputCorrect = false;
            }
            else if (lindaInstanceStruct.ret != null && lindaInstanceStruct.ret == 255) {
                result.error++;
            }
            else {
                result.waiting++;
                //kill processess that are still running
                lindaInstanceStruct.process.destroyForcibly();
            }
            printErrorStream(lindaInstanceStruct);
        }
        cleanTmpFiles();
        result.isOutputCorrect = isOutputCorrect;
        return result;
    }

    public class Result {

        public int finished;
        public int waiting;
        public int error;

        public boolean isOutputCorrect;

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

    private void cleanTmpFiles() {
        try {
            Process cleaner = Runtime.getRuntime().exec(new String[] { "sh", "-c", "rm " + Constants.msgFilePrefix + "*" });
            cleaner.waitFor();
            cleaner = Runtime.getRuntime().exec(new String[] { "sh", "-c", "rm " + Constants.tupleFilePath});
            cleaner.waitFor();
            cleaner = Runtime.getRuntime().exec(new String[] { "sh", "-c", "rm " + Constants.procFilePath});
            cleaner.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void printErrorStream(LindaInstanceStruct lindaInstanceStruct) {
        if (lindaInstanceStruct.error != null && !lindaInstanceStruct.error.equals("")) {
            System.out.println("Error: " + lindaInstanceStruct.lindaTypeEnum + " " + lindaInstanceStruct.pattern);
            System.out.println("CERR: " + lindaInstanceStruct.error);
        }
    }
}
