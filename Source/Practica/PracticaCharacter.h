// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PracticaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class APracticaCharacter : public ACharacter // Define la clase APracticaCharacter, que hereda de ACharacter (personaje con movimiento basico en UE).
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	// Variable que define el brazo (SpringArm) de la camara.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	// Variable que define la camara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	// Definicion del Input Action para el salto
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	// Definicion del Input Action para el movimiento
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	// Deficinion del Input Action que controla la rotacion de la camara
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	// Deficinion del Input Action para girar la camara utilizando el movimiento del mouse.
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;


	/*---------------------------
			  PARTE B
	-----------------------------*/

	// Declare el input Action que luego voy a asignar desde el editor
	// Lo que va a hacer este action es que cada vez que pressione la tecla se va a mostar un mesaje
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShowMessageAction;

	// Igual que arriba declare este input que ejecutara un dash
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

	//declare el input que ejecura el agachado
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

public:

	/** Constructor */
	APracticaCharacter();	

protected:

	/** Initialize input action bindings */
	// Se encarga de vincular los Input Action del jugador con las funciones que las ejecutan (para decirle a Unreal que funcion de tu personaje se ejecuta cuando el jugador aprieta un boton).
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	// Funcion que se ejecuta cada vez que el jugador mueve el stick o usa WASD en el teclado.
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	// funcion que se ejecuta cuando el jugador mueve la camara.
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	// Funcion que permite mover al personaje
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	// Funcion que permite rotar la camara
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	// Funcion que se llama cuando el jugador presiona el boton de salto.
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	// Funcion que ejecuta el final de un salto, cuando se suelta el boton
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/*---------------------------
	          PARTE B
	-----------------------------*/

	// Declare la funcion que muestra un mensaje en pantalla para bindearla desde C++
	UFUNCTION(BlueprintCallable, Category = "Input")
	void ShowMessage();

	// Igual que arriba declare la funcion que ejecutara un dash para bindearla en C++
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Dash();

	// Funcion que activa el crouch para bindearla en C++
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoCrouchStart();

	// Declare la funcion que desactiva el crouch para bindearla desde C++
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoCrouchEnd();

public:

	/** Returns CameraBoom subobject **/
	// Es una funcion llamada GetCameraBoom() que devuelve el puntero al componente CameraBoom (devuelve el puntero al componente)
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	// Es una funcion llamada GetFollowCamera() que devuelve el puntero a la cámara principal (FollowCamera) (devuelve el puntero al componente)
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

