// Copyright Epic Games, Inc. All Rights Reserved.

#include "PracticaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Practica.h"

APracticaCharacter::APracticaCharacter()
{
	// Set size for collision capsule
	// Configura el tamaño de la capsula de colision
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	// Indica que el personaje no debe rotar con el controlador, asi9 la camara rota independiente del personaje.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	// Configura el movimiento del personaje
	// Se orienta automaticamente hacia la direccion en la que se mueve.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Define la velocidad a la que puede rotar el personaje para alinearse con el movimiento.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	// Define parametros basicos de movimiento:
	GetCharacterMovement()->JumpZVelocity = 500.f; // Fuerza del salto.
	GetCharacterMovement()->AirControl = 0.35f; // Que tanto puede moverse en el aire
	GetCharacterMovement()->MaxWalkSpeed = 500.f; // Velocidad maxima al caminar
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f; //Velocidad minima detectable en joysticks analogicos.
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // Que tan rapido se frena al dejar de moverse
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f; // Que tan rapido se desacelera en el aire.

	// Create a camera boom (pulls in towards the player if there is a collision)
	// Crea el brazo de camara (spring arm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // Distancia de la camara al personaje
	CameraBoom->bUsePawnControlRotation = true; // La camara rota con el controlador

	// Create a follow camera
	// Crea la camara que sigue al personaje
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APracticaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	// Comprueba si el componente de input es del tipo mejorado (Enhanced Input)
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Aca se asignan las teclas

		// Jumping
		// Cuando se presiona el boton llama a la funcion saltar
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// Cuando se suelta el boton llama a la funcion de detener salto
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		// Asigna accion de movimiento
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APracticaCharacter::Move);
		// Asigna accion de movimiento de camara usando el mouse
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APracticaCharacter::Look);

		// Looking
		// Asigna accion de movimiento de camara usando joystick
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APracticaCharacter::Look);

		/*---------------------------
				  PARTE B
		-----------------------------*/

		// Bind de la accion mostrar mensaje
		EnhancedInputComponent->BindAction(ShowMessageAction, ETriggerEvent::Started, this, &APracticaCharacter::ShowMessage);
		
		// Bind del dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &APracticaCharacter::Dash);

		// Bind del crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APracticaCharacter::DoCrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APracticaCharacter::DoCrouchEnd);

	}
	else
	{
		// Si no encuentra el sistema Enhanced Input muestra un error en la consola
		UE_LOG(LogPractica, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APracticaCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	// Convierte el input en un vector 2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	// Llama a DoMove con esos valores (el DoMove es una funcion personalizada donde realmente se calcula hacia que direccion moverse y se aplica con AddMovementInput)
	DoMove(MovementVector.X, MovementVector.Y);
}

void APracticaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	// Convierte el input en un vector 2D (X=yaw, Y=pitch)
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	// Llama a DoLook con esos valores (e� DoLook es una funcion personalizada que realmente aplica esa rotacion usando AddControllerYawInput(Yaw) y AddControllerPitchInput(Pitch)
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void APracticaCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		// Obtiene la rotacion actual del controlador
		const FRotator Rotation = GetController()->GetControlRotation();

		// Obtiene solo la rotacion en el Yaw
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		// Calcula la direccion adelante en el mundo segun la camara
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		// Calcula la direccion "derecha" en el mundo
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		// Aplica movimiento en esas direcciones, escalado por los valores de entrada
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void APracticaCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		// Aplica la rotacion horizontal (Yaw) a la camara
		AddControllerYawInput(Yaw);
		// Aplica la rotacion vertical (Pitch) a la camara.
		AddControllerPitchInput(Pitch);
	}
}

void APracticaCharacter::DoJumpStart()
{
	// signal the character to jump
	// Llama a la funcion de salto de ACharacter
	Jump();
}

void APracticaCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	// Llama a la funcion que detiene el salto de ACharacter
	StopJumping();
}

/*---------------------------
		  PARTE B
-----------------------------*/

// defini la funcion para mostar un mensaje en la pantalla
void APracticaCharacter::ShowMessage()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Boton de mensaje presionado"));
}

// defini la funcion para hacer un dash, este tambien mostrara un mesaje cada vez que se ejecute la accion
void APracticaCharacter::Dash()
{
	FVector Forward = GetActorForwardVector();
	LaunchCharacter(Forward * 2500.0f, true, true);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Se activo el dash"));
}

// Activa el agachado
void APracticaCharacter::DoCrouchStart()
{
	Crouch();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Se activo el crouch"));
}

// Desactiva el agachado
void APracticaCharacter::DoCrouchEnd()
{
	UnCrouch();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Se desactivo el crouch"));
}