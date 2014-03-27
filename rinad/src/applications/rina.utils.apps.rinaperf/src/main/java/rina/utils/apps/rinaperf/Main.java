package rina.utils.apps.rinaperf;

import java.util.Arrays;

import eu.irati.librina.ApplicationProcessNamingInformation;

/**
 * Here the various options you can specify when starting Rinaperf as either a client or a server.
 
-sappname AP -sinstance AI

The AP and AI that Echo app will register as (if in the SERVER role) or connect to (if in the CLIENT role).

-cappname AP -cinstance AI

The AP and AI that the Echo client will use

-role (client|server) -client -server

Which role the application will take in the test

-sdusize N

The size of a single SDU

-time T

Duration of the test

All of the above can have implementation-dependant defaults. For example, the default for -appname might be "RINAperf" 
and the default for -instance might be the DIF address of the IPC Process.
 * @author eduardgrasa
 *
 */
public class Main {

	static {
		System.loadLibrary("rina_java");
	}
	
	public static final String ARGUMENT_SEPARATOR = "-";
	public static final String ROLE = "role";
	public static final String SDUSIZE = "sdusize";
	public static final String SERVER = "server";
	public static final String CLIENT = "client";
	public static final String SAPINSTANCE = "sinstance";
	public static final String SAPNAME = "sapname";
	public static final String CAPINSTANCE = "cinstance";
	public static final String CAPNAME = "capname";
	public static final String TIME = "time";
	
	public static final int DEFAULT_SDU_SIZE_IN_BYTES = 1500;
	public static final String DEFAULT_ROLE = SERVER;
	public static final String DEFAULT_SERVER_AP_NAME = "rina.utils.apps.rinaperf.server";
	public static final String DEFAULT_CLIENT_AP_NAME = "rina.utils.apps.rinaperf.client";
	public static final String DEFAULT_AP_INSTANCE = "1";
	public static final int DEFAULT_TIME_IN_SECONDS = 10;
	
	public static final String USAGE = "java -jar rina.utils.apps.rinaperf [-role] (client|server)" +
			"[-sapname] serverApName [-sinstance] serverApInstance [-capname] clientApName " + 
			"[-cinstance] clientApInstance [-sdusize] sduSizeInBytes [-time] testDurationInSeconds";
	public static final String DEFAULTS = "The defaults are: role=server;  sapname=rina.utils.apps.rinaperf.server; " + 
			"sinstance=1; capname=rina.utils.apps.echo.client; cinstance=1; sdusize=1500; time=10";
	
	public static void main(String[] args){
		System.out.println(Arrays.toString(args));
		
		int sduSizeInBytes = DEFAULT_SDU_SIZE_IN_BYTES;
		boolean server = false;
		String serverApName = DEFAULT_SERVER_AP_NAME;
		String clientApName = DEFAULT_CLIENT_AP_NAME;
		String serverApInstance = DEFAULT_AP_INSTANCE;
		String clientApInstance = DEFAULT_AP_INSTANCE;
		int time = DEFAULT_TIME_IN_SECONDS;
		
		int i=0;
		while(i<args.length){
			if (args[i].equals(ARGUMENT_SEPARATOR + ROLE)){
				if (args[i+1].equals(CLIENT)){
					server = false;
				}else if (args[i+1].equals(SERVER)){
					server = true;
				}else{
					showErrorAndExit(ROLE);
				}
			}else if (args[i].equals(ARGUMENT_SEPARATOR + SAPNAME)){
				serverApName = args[i+1];
			}else if (args[i].equals(ARGUMENT_SEPARATOR + SAPINSTANCE)){
				serverApInstance = args[i+1];
			}else if (args[i].equals(ARGUMENT_SEPARATOR + CAPNAME)){
				clientApName = args[i+1];
			}else if (args[i].equals(ARGUMENT_SEPARATOR + CAPINSTANCE)){
				clientApInstance = args[i+1];
			}else if (args[i].equals(ARGUMENT_SEPARATOR + SDUSIZE)){
				try{
					sduSizeInBytes = Integer.parseInt(args[i+1]);
					if (sduSizeInBytes <1){
						showErrorAndExit(SDUSIZE);
					}
				}catch(Exception ex){
					showErrorAndExit(SDUSIZE);
				}
			}else if (args[i].equals(ARGUMENT_SEPARATOR + TIME)){
				try{
					time = Integer.parseInt(args[i+1]);
					if (time < 10){
						showErrorAndExit(TIME);
					}
				}catch(Exception ex){
					showErrorAndExit(TIME);
				}
			}else{
				System.out.println("Wrong argument.\nUsage: "
						+USAGE+"\n"+DEFAULTS);
				System.exit(-1);
			}
			
			i = i+2;
		}
		
		ApplicationProcessNamingInformation serverAPNamingInfo = 
				new ApplicationProcessNamingInformation(serverApName, serverApInstance);
		ApplicationProcessNamingInformation clientApNamingInfo = 
				new ApplicationProcessNamingInformation(clientApName, clientApInstance);
		
		RINAPerf rinaPerf = new RINAPerf(server, serverAPNamingInfo, clientApNamingInfo,
				sduSizeInBytes, time);
		rinaPerf.execute();
	}
	
	public static void showErrorAndExit(String parameterName){
		System.out.println("Wrong value for argument "+parameterName+".\nUsage: "
				+USAGE+"\n"+DEFAULTS);
		System.exit(-1);
	}

}
