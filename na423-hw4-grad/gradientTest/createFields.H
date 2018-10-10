    Info<< "Reading field T\n" << endl;

    volScalarField T
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );


    Info<< "Reading transportProperties\n" << endl;

    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );


    Info<< "Reading diffusivity DT\n" << endl;

    dimensionedScalar DT
    (
        transportProperties.lookup("DT")
    );


   forAll(T, cellI)
   {
     T[cellI] = Foam::sin(3.14159*0.5*(T.mesh().C()[cellI] & vector(0,1,0)));
   };

   forAll(T.boundaryField(), patchI)
   {
        forAll(T.boundaryField()[patchI], faceI)
        {
          T.boundaryField()[patchI][faceI] = Foam::sin(3.14159*0.5*
                                  (T.mesh().Cf().boundaryField()[patchI][faceI] & vector(0,1,0)));
        }
   };
