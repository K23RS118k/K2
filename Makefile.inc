APPL_COBJS +=

APPL_CXXOBJS +=	BluetoothLogger.o \
				EV3ColorSensor.o \
				EV3SonarSensor.o \
				RearMotor.o \
				ArmControl.o \
				TimerJudgement.o \
				CalcCurrentLocation.o \
				DistanceJudgement.o \
				StraightDetection.o \
				RunParameter.o \
				SpeedAdjustment.o \
				Tactics.o \
				SectionControlTactics.o \
				Action.o \
				LineTraceAction.o \
				EdgeChangeAction.o \
				ArmPositionSetAction.o \
				RunStraightAction.o \
				RotateMachineAction.o \
				CurvatureRunAction.o \
				DecelerationRotaryAction.o \
				Calibration.o \
				IPCommunication.o \
				
				

SRCLANG := c++

INCLUDES += -I$(ETROBO_HRP3_WORKSPACE)/etroboc_common

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile
include $(EV3RT_SDK_LIB_DIR)/lib2/Makefile

endif
