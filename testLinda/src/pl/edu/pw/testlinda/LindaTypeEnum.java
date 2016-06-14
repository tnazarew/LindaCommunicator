package pl.edu.pw.testlinda;

public enum LindaTypeEnum {
    OUTPUT,
    INPUT,
    READ;

    public String toString()
    {
        switch (this) {
            case OUTPUT:
                return "output";
            case INPUT:
                return "input";
            case READ:
                return "read";
        }
        return null;
    }
}
