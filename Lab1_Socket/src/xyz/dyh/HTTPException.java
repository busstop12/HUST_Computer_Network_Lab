package xyz.dyh;

public class HTTPException extends Exception{
    public int status;
    public String msg;
    HTTPException(int status, String msg) {
        this.status = status;
        this.msg = msg;
    }
}
