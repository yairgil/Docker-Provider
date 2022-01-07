import java.io.IOException;

public class Program {

    public static void main(String[] args) throws InterruptedException
    {
	long seqNumber = 0;

        double mbPerMinute = Double.parseDouble(System.getenv("MB_LOGGED_PER_MINUTE"));
        int logLineSize = Integer.parseInt(System.getenv("LOG_LINE_LENGTH_BYTES"));
        long maxLines = Long.parseLong(System.getenv("EXIT_AFTER_N_LINES"));  // set to 0 to never stop

        long nextTimeMillis = System.currentTimeMillis() + 1000;
        long linesPrinted = 0;

        while (maxLines == 0 || linesPrinted < maxLines) {
            int bytesPrinted = 0;
            while (bytesPrinted < mbPerMinute * 1000 * 1000 / 60 && (maxLines == 0 || linesPrinted < maxLines)) {
                String logMessage = "Sequence number=" + seqNumber + " random data:";
                seqNumber += 1;
                // pad the log message with 0123456789 repeated (this will be a little too big)
                logMessage = logMessage + new String(new byte[(logLineSize - logMessage.length())/10 + 1]).replace("\u0000", "0123456789");

                // trim the log message to the exact right length (subtract 1 because println will add \n to the end)
                logMessage = logMessage.substring(0, logLineSize - 1);

                bytesPrinted += logMessage.length();
                linesPrinted++;
                System.out.println(logMessage);
            }
            while (System.currentTimeMillis() < nextTimeMillis) {
                Thread.sleep(1);
            }

            nextTimeMillis += 1000;
        }
    }

}
