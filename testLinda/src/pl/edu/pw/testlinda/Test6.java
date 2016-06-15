package pl.edu.pw.testlinda;

import java.util.ArrayList;
import java.util.Random;

public class Test6 extends Test{
    int n;

    Test6(int n) {
        this.n = n;
        this.description = "Test runs random number of output, read and input processes with tuple and matching pattern." +
                "Number of the processes sums up to " + n + ". Read and input processes are chosen more often than output.";
    }

    @Override
    public boolean run() {
        String tuple = "2,\"a\",4.0,\"a\"";

        String pattern = "i:2,s:*,f:*,s:*";

        ArrayList<LindaInstanceStruct> outputList = new ArrayList<>();
        ArrayList<LindaInstanceStruct> readList = new ArrayList<>();
        ArrayList<LindaInstanceStruct> inputList = new ArrayList<>();

        Random random = new Random();
        int r;

        for (int i = 0; i < n; i++) {
            if ((r = random.nextInt(6)) == 0) {
                outputList.add(startInstance(LindaTypeEnum.OUTPUT, tuple));
            } else if (r%2 == 0) {
                readList.add(startInstance(LindaTypeEnum.READ, pattern));
            } else {
                inputList.add(startInstance(LindaTypeEnum.INPUT, pattern));
            }
        }

        try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Result outputResult = checkResultsAndClean(outputList);
        Result readResult = checkResultsAndClean(readList, tuple);
        Result inputResult = checkResultsAndClean(inputList, tuple);

        System.out.println("\t Output result: " + outputResult.getString());
        System.out.println("\t Read result: " + readResult.getString());
        System.out.println("\t Input result: " + inputResult.getString());

        if (outputResult.finished >= inputResult.finished && outputResult.finished >= readResult.finished) {
            return true;
        }
        else {
            return false;
        }
    }
}
